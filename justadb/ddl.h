#pragma once

#include <string>
#include <unordered_map>
#include <vector>

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
