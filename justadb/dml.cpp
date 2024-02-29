#include "dml.h"

namespace JustADb {

auto DmlQueryExec::ExecuteSelectQuery(const SelectQuery &query)
    -> Result<std::vector<const Tuple *>> {
  if (query.columns().empty()) {
    return Error("No columns are selected");
  }

  std::optional<const Table *> table = this->db_.GetTable(query.table().name());
  if (!table.has_value()) {
    return Error("Table not found");
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
      return Error("No column found");
    }
  }

  return result;
}

auto DmlQueryExec::ExecuteInsertQuery(const InsertQuery &query)
    -> Result<std::vector<const Tuple *>> {
  return Error("Not implemented");
}

auto DmlQueryExec::ExecuteUpdateQuery(const UpdateQuery &query) -> Result<int> {
    return Error("Not implemented");
}

auto DmlQueryExec::ExecuteDeleteQuery(const JustADb::DeleteQuery &query) -> Result<int> {
    return Error("Not implemented");
}

} // namespace JustADb
