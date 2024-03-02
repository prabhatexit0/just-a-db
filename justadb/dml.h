#pragma once

#include "ddl.h"
#include "utils.h"
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <expected>

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
      : column_(std::move(column)), value_(std::move(value)), op_(op) {}

  [[nodiscard]] auto column() const {
    return column_;
  }
  [[nodiscard]] auto value() const {
    return value_;
  }
  [[nodiscard]] auto op() const {
    return op_;
  }

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
      : column_(std::move(column)), order_(order) {}

  [[nodiscard]] auto column() const {
    return column_;
  }
  [[nodiscard]] auto order() const {
    return order_;
  }

private:
  std::string column_;
  Order order_;
};

class DmlQuery {
public:
  enum class Kind { SELECT, INSERT, UPDATE, DELETE };

  DmlQuery(Kind kind, Table table, std::vector<WhereClause> where_clause = {})
      : kind_(kind), table_(std::move(table)),
        where_clauses_(std::move(where_clause)) {}

  DmlQuery &Where(const std::string &column, const std::string &value,
                  WhereClause::Operator op) {
    where_clauses_.emplace_back(column, value, op);
    return *this;
  }

  [[nodiscard]] auto kind() const {
    return kind_;
  }
  [[nodiscard]] auto table() const {
    return table_;
  }
  [[nodiscard]] auto where_clause() const {
    return where_clauses_;
  }

private:
  Kind kind_;
  Table table_;

  // TODO: Think about generalizing this to filters.
  std::vector<WhereClause> where_clauses_;
};

class SelectQuery : public DmlQuery {
public:
  SelectQuery(Table table, std::vector<Column> columns)
      : DmlQuery(Kind::SELECT, std::move(table)), columns_(std::move(columns)) {
  }

  SelectQuery &OrderBy(std::string column, OrderByClause::Order order) {
    orderByClause_ = OrderByClause(std::move(column), order);
    return *this;
  }

  [[nodiscard]] auto orderByClause() const {
    return orderByClause_;
  }

  [[nodiscard]] auto columns() const {
    return columns_;
  }

private:
  std::optional<OrderByClause> orderByClause_;
  std::vector<Column> columns_;
};

class InsertQuery : public DmlQuery {
public:
  InsertQuery(Table table, std::unordered_map<std::string, Value> insert_values,
              std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::INSERT, std::move(table), std::move(where_clauses)),
        insert_values_(std::move(insert_values)) {}

private:
  // Key is column name, value is the value to insert.
  std::unordered_map<std::string, Value> insert_values_;
};

class UpdateQuery : public DmlQuery {
public:
  UpdateQuery(Table table,
              const std::unordered_map<std::string, Value> &columns,
              std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::UPDATE, std::move(table), std::move(where_clauses)) {}

private:
  std::unordered_map<std::string, Value> set_values_;
};

class DeleteQuery : public DmlQuery {
public:
  explicit DeleteQuery(Table table)
      : DmlQuery(Kind::DELETE, std::move(table)) {}

  DeleteQuery(Table table, std::vector<WhereClause> where_clauses)
      : DmlQuery(Kind::DELETE, std::move(table), std::move(where_clauses)) {}
};

class DmlQueryExec {
public:
  explicit DmlQueryExec(Database &db) : db_(db) {}

  auto ExecuteSelectQuery(const SelectQuery &query)
      -> std::expected<std::vector<const Tuple *>, Error>;
  auto ExecuteInsertQuery(const InsertQuery &query)
      -> std::expected<std::vector<const Tuple *>, Error>;
  auto ExecuteUpdateQuery(const UpdateQuery &query) -> std::expected<int, Error>;
  auto ExecuteDeleteQuery(const DeleteQuery &query) -> std::expected<int, Error>;

private:
  Database &db_;
};

} // End namespace JustADb
