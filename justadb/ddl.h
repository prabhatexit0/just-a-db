#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JustADb {

class Value {
public:
  enum class Type { INT, STRING, BOOL, FLOAT };

  Value(std::variant<std::string, int, float, bool> value) : value_(value) {}

  auto type() const { return type_; }
  auto value() const { return value_; }

private:
  Type type_;
  std::variant<std::string, int, float, bool> value_;
};

class Column {
public:
  Column(std::string name, Value::Type type) : name_(name), type_(type) {}

  auto name() const { return name_; }
  auto type() const { return type_; }

private:
  std::string name_;
  Value::Type type_;
};

class Tuple {
public:
  Tuple() : value_map_({}) {}
  Tuple(std::unordered_map<std::string, Value &> value_map)
      : value_map_(value_map) {}

  auto values_map() const { return value_map_; }

  auto get_value(std::string column_name) const -> std::optional<Value> {
    auto value = value_map_.find(column_name);
    if (value != value_map_.end()) {
      return value->second;
    }
    return std::nullopt;
  }

  void insert_value(std::string column_name, Value &value) {
    value_map_.insert({column_name, value});
  }

  auto is_empty() const { return value_map_.empty(); }

private:
  std::unordered_map<std::string, Value &> value_map_;
};

class Table {
public:
  Table(std::string name, std::vector<Column> columns,
        std::vector<Tuple> tuples)
      : name_(name), columns_(columns), tuples_(tuples) {}

  auto name() const { return name_; }
  auto columns() const { return columns_; }
  auto tuples() const { return tuples_; }

private:
  std::string name_;
  std::vector<Column> columns_;
  std::vector<Tuple> tuples_;
};

class DdlQuery {
public:
  enum class Type { CREATE_TABLE, DROP_TABLE, ALTER_TABLE };
  DdlQuery(Type type, std::string table_name)
      : type_(type), table_name_(table_name) {}

  auto type() const { return type_; }

private:
  Type type_;
  std::string table_name_;
};

class CreateTableQuery : public DdlQuery {
public:
  CreateTableQuery(std::string table_name, std::vector<Column> columns)
      : DdlQuery(Type::CREATE_TABLE, table_name), columns_(columns) {}

  auto columns() const { return columns_; }

private:
  std::vector<Column> columns_;
};

class DropTableQuery : public DdlQuery {
public:
  DropTableQuery(std::string table_name)
      : DdlQuery(Type::DROP_TABLE, table_name) {}
};

class AlterTableQuery : public DdlQuery {
public:
  enum class AlterType { ADD_COLUMN, DROP_COLUMN };
  AlterTableQuery(std::string table_name, AlterType alter_type, Column column)
      : DdlQuery(Type::ALTER_TABLE, table_name), alter_type_(alter_type),
        column_(column) {}

  auto alter_type() const { return alter_type_; }
  auto column() const { return column_; }

private:
  AlterType alter_type_;
  Column column_;
};

} // namespace JustADb
