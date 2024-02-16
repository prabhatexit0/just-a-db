#pragma once

#include <optional>
#include <string>
#include <vector>

class WhereClause {
public:
  enum class Operator {
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    LIKE,
    NOT_LIKE,
    IN,
    NOT_IN,
    IS_NULL,
    IS_NOT_NULL
  };

  WhereClause(std::string column, std::string value, Operator op)
      : column_(column), value_(value), op_(op) {}

  auto column() const { return column_; }
  auto value() const { return value_; }
  auto op() const { return op_; }

private:
  std::string column_;
  std::string value_;
  Operator op_;
};

class OrderByClause {
public:
  enum class Order {
    ASCENDING,
    DESCENDING,
  };

  OrderByClause(std::string column, Order order)
      : column_(column), order_(order) {}

  auto column() const { return column_; }
  auto order() const { return order_; }

private:
  std::string column_;
  Order order_;
};

class DmlQuery {
public:
  enum class Kind { SELECT, INSERT, UPDATE, DELETE };

  DmlQuery(Kind kind, std::string table, std::vector<std::string> columns)
      : kind_(kind), table_(table), columns_(columns) {}

  DmlQuery &Where(std::string column, std::string value,
                  WhereClause::Operator op) {
    whereClause_ = WhereClause(column, value, op);
    return *this;
  }

  DmlQuery &OrderBy(std::string column, OrderByClause::Order order) {
    orderByClause_ = OrderByClause(column, order);
    return *this;
  }

  auto kind() const { return kind_; }
  auto table() const { return table_; }
  auto columns() const { return columns_; }
  auto whereClause() const { return whereClause_; }
  auto orderByClause() const { return orderByClause_; }

private:
  Kind kind_;
  std::string table_;
  std::vector<std::string> columns_;
  std::optional<WhereClause> whereClause_;
  std::optional<OrderByClause> orderByClause_;
};

class SelectQuery : public DmlQuery {
public:
  SelectQuery(std::string table, std::vector<std::string> columns)
      : DmlQuery(Kind::SELECT, table, columns) {}
};

class InsertQuery : public DmlQuery {
public:
  InsertQuery(std::string table, std::vector<std::string> columns)
      : DmlQuery(Kind::INSERT, table, columns) {}
};

class UpdateQuery : public DmlQuery {
public:
  class SetClause {
  public:
    SetClause(std::string column, std::string value)
        : column_(column), value_(value) {}

  private:
    std::string column_;
    std::string value_;
  };

  UpdateQuery(SetClause set_clause, std::string table,
              std::vector<std::string> columns)
      : DmlQuery(Kind::UPDATE, table, columns), set_clause_(set_clause) {}

  auto set_clause() const { return set_clause_; }

private:
  SetClause set_clause_;
};

class DeleteQuery : public DmlQuery {
public:
  DeleteQuery(std::string table, std::vector<std::string> columns)
      : DmlQuery(Kind::DELETE, table, columns) {}
};
