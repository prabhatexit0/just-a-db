#pragma once

#include "utils.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace JustADb {

    class Value {
    public:
        explicit Value(std::variant<std::string, int, float, bool> value) : value_(std::move(value)) {}

        [[nodiscard]] auto value() const { return value_; }

    private:
        std::variant<std::string, int, float, bool> value_;
    };

    class Column {
    public:
        enum class Type {
            INT, STRING, BOOL, FLOAT
        };

        Column(std::string name, Type type) : name_(std::move(name)), type_(type) {}

        [[nodiscard]] auto name() const { return name_; }

        [[nodiscard]] auto type() const { return type_; }

        auto set_type(Type type) -> const Column * {
            type_ = type;
            return this;
        }

        auto rename_column(std::string name) -> Column * {
            name_ = std::move(name);
            return this;
        }

    private:
        std::string name_;
        Type type_;
    };

    class Tuple {
    public:
        Tuple() : value_map_({}) {}

        explicit Tuple(std::unordered_map<std::string, const Value *> value_map)
                : value_map_(std::move(value_map)) {}

        [[nodiscard]] auto GetValue(const std::string &column_name) const -> std::optional<const Value *>;

        void InsertValue(const std::string &column_name, const Value *value);

        [[nodiscard]] auto is_empty() const { return value_map_.empty(); }

        [[nodiscard]] auto values_map() const { return value_map_; }

    private:
        std::unordered_map<std::string, const Value *> value_map_;
    };

    class Table {
    public:
        Table(std::string name, std::vector<Column *> columns)
                : name_(std::move(name)), columns_(std::move(columns)), tuples_({}) {}

        auto InsertTuple(Tuple *tuple) -> Result<const Tuple *>;

        auto SelectTuple(const std::string &column_name, const Value *value) const
        -> std::vector<const Tuple *>;

        [[nodiscard]] auto SelectAll() const -> std::vector<Tuple *> { return tuples_; }

        auto AddColumn(Column *column) -> Result<const Table *>;

        auto DropColumn(const std::string &column_name) -> Result<const Table *>;

        [[nodiscard]] auto GetColumn(const std::string &name) const -> std::optional<const Column *> {
            for (const auto *column: columns_) {
                if (column->name() == name) {
                    return column;
                }
            }
            return std::nullopt;
        }

        [[nodiscard]] auto name() const { return name_; }

        [[nodiscard]] auto columns() const { return columns_; }

        [[nodiscard]] auto tuples() const { return tuples_; }

    private:
        std::string name_;
        std::vector<Column *> columns_;
        std::vector<Tuple *> tuples_;
    };

    class DdlQuery {
    public:
        enum class Type {
            CREATE_DB,
            DROP_DB,
            CREATE_TABLE,
            DROP_TABLE,
            ALTER_TABLE,
            USE_DB
        };

        explicit DdlQuery(Type type) : type_(type) {}

        [[nodiscard]] auto type() const { return type_; }

    private:
        Type type_;
    };

    class CreateDatabaseQuery : public DdlQuery {
    public:
        explicit CreateDatabaseQuery(std::string db_name)
                : DdlQuery(Type::CREATE_DB), db_name_(std::move(db_name)) {}

        [[nodiscard]] auto db_name() const { return db_name_; }

    private:
        std::string db_name_;
    };

    class UseDatabaseQuery : public DdlQuery {
    public:
        explicit UseDatabaseQuery(std::string db_name)
                : DdlQuery(Type::USE_DB), db_name_(std::move(db_name)) {}

        [[nodiscard]] auto db_name() const { return db_name_; }

    private:
        std::string db_name_;
    };

    class DropDatabaseQuery : public DdlQuery {
    public:
        explicit DropDatabaseQuery(const std::string &db_name) : DdlQuery(Type::DROP_DB) {}

        [[nodiscard]] auto db_name() const { return db_name_; }

    private:
        std::string db_name_;
    };

    class CreateTableQuery : public DdlQuery {
    public:
        CreateTableQuery(std::string table_name, std::vector<Column *> columns)
                : DdlQuery(Type::CREATE_TABLE), table_name_(std::move(table_name)),
                  columns_(std::move(columns)) {}

        [[nodiscard]] auto columns() const { return columns_; }

        [[nodiscard]] auto table_name() const { return table_name_; }

    private:
        std::string table_name_;
        std::vector<Column *> columns_;
    };

    class DropTableQuery : public DdlQuery {
    public:
        explicit DropTableQuery(std::string table_name)
                : DdlQuery(Type::DROP_TABLE), table_name_(std::move(table_name)) {}

        [[nodiscard]] auto table_name() const { return table_name_; }

    private:
        std::string table_name_;
    };

    class AlterTableQuery : public DdlQuery {
    public:
        enum class AlterType {
            ADD_COLUMN,
            DROP_COLUMN,
            RENAME_COLUMN,
            MODIFY_DATATYPE
        };

        AlterTableQuery(std::string table_name, AlterType alter_type,
                        Column *column)
                : DdlQuery(Type::ALTER_TABLE), table_name_(std::move(table_name)),
                  alter_type_(alter_type), column_(column) {}

        [[nodiscard]] auto alter_type() const { return alter_type_; }

        [[nodiscard]] auto column() const { return column_; }

        [[nodiscard]] auto table_name() const { return table_name_; }

    private:
        std::string table_name_;
        AlterType alter_type_;
        Column *column_;
    };

    class Database {
    public:
        explicit Database(std::string name) : name_(std::move(name)) {}

        auto CreateTable(const std::string &name, std::vector<Column *> columns)
        -> Result<const Table *>;

        auto DropTable(const std::string &table_name) -> Result<bool>;

        auto UpdateTable(const std::string &table_name, Table *table)
        -> Result<const Table *>;

        [[nodiscard]] auto GetTable(const std::string &name) const -> std::optional<const Table *>;

        [[nodiscard]] auto GetModifiableTable(const std::string &name) const -> std::optional<Table *>;

        [[nodiscard]] auto name() const { return name_; }

    private:
        std::string name_;
        std::unordered_map<std::string, Table *> tables_;
    };

    class DatabaseManager {
    public:
        auto CreateDatabase(const std::string &name) -> Result<const Database *>;

        auto DropDatabase(const std::string &name) -> Result<bool>;

        [[nodiscard]] auto GetDatabase(const std::string &name) const -> std::optional<Database *>;

        auto SetCurrentDatabase(Database *db) -> Result<bool> {
            if (databases_.find(db->name()) == databases_.end()) {
                return Error("Database does not exist");
            }

            current_database_ = db;
            return true;
        }

        auto current_database() -> std::optional<Database *> {
            return current_database_;
        }

    private:
        std::unordered_map<std::string, Database *> databases_;
        std::optional<Database *> current_database_;
    };

    class DdlQueryExec {
    public:
        explicit DdlQueryExec(DatabaseManager *db_manager) : db_manager_(db_manager) {}

        auto ExecuteUseDatabaseQuery(const UseDatabaseQuery &query) -> Result<bool>;

        auto ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
        -> Result<const Database *>;

        auto ExecuteDropDatabaseQuery(const DropDatabaseQuery &query) -> Result<bool>;

        auto ExecuteCreateTableQuery(const CreateTableQuery &query)
        -> Result<const Table *>;

        auto ExecuteDropTableQuery(const DropTableQuery &query) -> Result<bool>;

        auto ExecuteAlterTableQuery(const AlterTableQuery &query)
        -> Result<const Table *>;

        [[nodiscard]] auto db_manager() const { return db_manager_; }

    private:
        DatabaseManager *db_manager_;
    };

} // namespace JustADb
