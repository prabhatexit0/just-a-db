#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace JustADb {

class Value {
public:
  enum class Type { INT, STRING, DOUBLE, BOOL };
  Value(int value) : type_(Type::INT), intValue_(value) {}
  Value(std::string value) : type_(Type::STRING), stringValue_(value) {}
  Value(double value) : type_(Type::DOUBLE), doubleValue_(value) {}
  Value(bool value) : type_(Type::BOOL), boolValue_(value) {}

  auto type() const { return type_; }
  auto int_value() const { return intValue_; }
  auto string_value() const { return stringValue_; }
  auto double_value() const { return doubleValue_; }
  auto bool_value() const { return boolValue_; }

private:
  Type type_;
  int intValue_;
  std::string stringValue_;
  double doubleValue_;
  bool boolValue_;
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
  Tuple(std::unordered_map<Column, Value> values_map)
      : values_map_(values_map) {}

private:
  std::unordered_map<Column, Value> values_map_;
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
