#pragma once

#include "utils.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JustADb {

class Value {
public:
  Value(std::variant<std::string, int, float, bool> value) : value_(value) {}

  auto value() const { return value_; }

private:
  std::variant<std::string, int, float, bool> value_;
};

class Column {
public:
  enum class Type { INT, STRING, BOOL, FLOAT };

  Column(std::string name, Type type) : name_(name), type_(type) {}

  auto name() const { return name_; }
  auto type() const { return type_; }

private:
  std::string name_;
  Type type_;
};

class Tuple {
public:
  Tuple() : value_map_({}) {}
  Tuple(std::unordered_map<std::string, const Value *> value_map)
      : value_map_(value_map) {}

  auto GetValue(std::string column_name) const -> std::optional<const Value *>;
  void InsertValue(std::string column_name, const Value *value);

  auto is_empty() const { return value_map_.empty(); }

  auto values_map() const { return value_map_; }

private:
  std::unordered_map<std::string, const Value *> value_map_;
};

class Table {
public:
  Table(std::string name, std::vector<const Column *> columns)
      : name_(name), columns_(columns), tuples_({}) {}

  auto InsertTuple(const Tuple *tuple) -> Result<const Tuple *>;
  auto SelectTuple(std::string column_name, const Value *value) const
      -> std::vector<const Tuple *>;
  auto SelectAll() const -> std::vector<const Tuple *> { return tuples_; }
  auto AddColumn(const Column *column) -> Result<const Column *>;
  auto DropColumn(std::string column_name) -> Result<bool>;

  auto name() const { return name_; }
  auto columns() const { return columns_; }
  auto tuples() const { return tuples_; }

private:
  std::string name_;
  std::vector<const Column *> columns_;
  std::vector<const Tuple *> tuples_;
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
  DdlQuery(Type type) : type_(type) {}

  auto type() const { return type_; }

private:
  Type type_;
};

class CreateDatabaseQuery : public DdlQuery {
public:
  CreateDatabaseQuery(std::string db_name)
      : DdlQuery(Type::CREATE_DB), db_name_(db_name) {}

  auto db_name() const { return db_name_; }

private:
  std::string db_name_;
};

class UseDatabaseQuery : public DdlQuery {
public:
  UseDatabaseQuery(std::string db_name)
      : DdlQuery(Type::USE_DB), db_name_(db_name) {}

  auto db_name() const { return db_name_; }

private:
  std::string db_name_;
};

class DropDatabaseQuery : public DdlQuery {
public:
  DropDatabaseQuery(std::string db_name) : DdlQuery(Type::DROP_DB) {}

  auto db_name() const { return db_name_; }

private:
  std::string db_name_;
};

class CreateTableQuery : public DdlQuery {
public:
  CreateTableQuery(std::string table_name, std::vector<const Column *> columns)
      : DdlQuery(Type::CREATE_TABLE), table_name_(table_name),
        columns_(columns) {}

  auto columns() const { return columns_; }
  auto table_name() const { return table_name_; }

private:
  std::string table_name_;
  std::vector<const Column *> columns_;
};

class DropTableQuery : public DdlQuery {
public:
  DropTableQuery(std::string table_name)
      : DdlQuery(Type::DROP_TABLE), table_name_(table_name) {}

  auto table_name() const { return table_name_; }

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
                  const Column *column)
      : DdlQuery(Type::ALTER_TABLE), table_name_(table_name),
        alter_type_(alter_type), column_(column) {}

  auto alter_type() const { return alter_type_; }
  auto column() const { return column_; }
  auto table_name() const { return table_name_; }

private:
  std::string table_name_;
  AlterType alter_type_;
  const Column *column_;
};

class Database {
public:
  Database(std::string name) : name_(name) {}

  auto CreateTable(std::string name, std::vector<const Column *> columns)
      -> Result<const Table *>;
  auto DropTable(std::string table_name) -> Result<bool>;
  auto UpdateTable(std::string table_name, const Table *table)
      -> Result<const Table *>;
  auto GetTable(std::string name) const -> std::optional<const Table *>;

  auto name() const { return name_; }

private:
  std::string name_;
  std::unordered_map<std::string, const Table *> tables_;
};

class DatabaseManager {
public:
  auto CreateDatabase(std::string name) -> Result<const Database *>;
  auto DropDatabase(std::string name) -> Result<bool>;
  auto GetDatabase(std::string name) const -> std::optional<Database *>;

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
  DdlQueryExec(DatabaseManager *db_manager) : db_manager_(db_manager) {}

  auto ExecuteUseDatabaseQuery(const UseDatabaseQuery &query) -> Result<bool>;
  auto ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
      -> Result<const Database *>;
  auto ExecuteDropDatabaseQuery(const DropDatabaseQuery &query) -> Result<bool>;
  auto ExecuteCreateTableQuery(const CreateTableQuery &query)
      -> Result<const Table *>;
  auto ExecuteDropTableQuery(const DropTableQuery &query) -> Result<bool>;
  auto ExecuteAlterTableQuery(const AlterTableQuery &query)
      -> Result<const Table *>;

  auto db_manager() const { return db_manager_; }

private:
  DatabaseManager *db_manager_;
};

} // namespace JustADb
