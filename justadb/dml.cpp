#include "dml.h"

namespace JustADb {

auto DmlQueryExec::ExecuteSelectQuery(const SelectQuery &query)
    -> std::expected<std::vector<const Tuple *>, Error> {
  if (query.columns().empty()) {
    return std::unexpected(Error("No columns are selected"));
  }

  std::optional<const Table *> table = this->db_.GetTable(query.table().name());
  if (!table.has_value()) {
    return  std::unexpected(Error("Table not found"));
  }

  std::vector<const Tuple *> result;
  for (const auto &tuple : (*table)->tuples()) {
    Tuple new_tuple;
    for (const auto &column : query.columns()) {
      auto value = tuple->GetValue(column.name());
      if (value.has_value()) {
        new_tuple.InsertValue(column.name(), *value);
      }
    }

    if (new_tuple.is_empty()) {
      return  std::unexpected(Error("No column found"));
    }
  }

  return result;
}

auto DmlQueryExec::ExecuteInsertQuery(const InsertQuery &query)
    -> std::expected<std::vector<const Tuple *>, Error> {
  return  std::unexpected(Error("Not implemented"));
}

auto DmlQueryExec::ExecuteUpdateQuery(const UpdateQuery &query) -> std::expected<int, Error> {
  return  std::unexpected(Error("Not implemented"));
}

auto DmlQueryExec::ExecuteDeleteQuery(const JustADb::DeleteQuery &query)
    -> std::expected<int, Error> {
  return  std::unexpected(Error("Not implemented"));
}

} // namespace JustADb
