#pragma once

#include "ddl.h"
#include <optional>
#include <string>
#include <vector>

namespace JustADb {
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

  DmlQuery(Kind kind, Table table, std::vector<WhereClause> where_clause = {})
      : kind_(kind), table_(table), where_clauses_(where_clause) {}

  DmlQuery &Where(std::string column, std::string value,
                  WhereClause::Operator op) {
    where_clauses_.push_back(WhereClause(column, value, op));
    return *this;
  }

  auto kind() const { return kind_; }
  auto table() const { return table_; }
  auto where_clause() const { return where_clauses_; }

private:
  Kind kind_;
  Table table_;

  // TODO: Think about generalizing this to filters.
  std::vector<WhereClause> where_clauses_;
};

class SelectQuery : public DmlQuery {
public:
  SelectQuery(Table table, std::vector<Column> columns)
      : DmlQuery(Kind::SELECT, table), columns_(columns) {}

  SelectQuery &OrderBy(std::string column, OrderByClause::Order order) {
    orderByClause_ = OrderByClause(column, order);
    return *this;
  }

  auto orderByClause() const { return orderByClause_; }

  auto columns() const { return columns_; }

private:
  std::optional<OrderByClause> orderByClause_;
  std::vector<Column> columns_;
};

class InsertQuery : public DmlQuery {
public:
  InsertQuery(Table table, std::unordered_map<Column, Value> insert_values,
              std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::INSERT, table, where_clauses),
        insert_values_(insert_values) {}

private:
  std::unordered_map<Column, Value> insert_values_;
};

class UpdateQuery : public DmlQuery {
public:
  UpdateQuery(Table table, std::unordered_map<Column, Value> columns,
              std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::UPDATE, table, where_clauses) {}

private:
  std::unordered_map<Column, Value> set_values_;
};

class DeleteQuery : public DmlQuery {
public:
  DeleteQuery(Table table) : DmlQuery(Kind::DELETE, table) {}

  DeleteQuery(Table table, std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::DELETE, table, where_clauses) {}
};

} // End namespace JustADb
