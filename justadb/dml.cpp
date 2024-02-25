#include "dml.h"

namespace JustADb {

auto DmlQueryExec::ExecuteSelectQuery(const SelectQuery &query)
    -> Result<std::vector<const Tuple*>> {
  if (query.columns().empty()) {
    return Error("No columns are selected");
  }

  std::optional<const Table*> table = this->db_.get_table(query.table().name());
  if (!table.has_value()) {
    return Error("Table not found");
  }

  std::vector<const Tuple*> result;
  for (const auto &tuple : (*table)->tuples()) {
    Tuple new_tuple;
    for (const auto &column : query.columns()) {
      auto value = tuple->get_value(column.name());
      if (value.has_value()) {
        new_tuple.insert_value(column.name(), *value);
      }
    }

    if (new_tuple.is_empty()) {
      return Error("No column found");
    }
  }

  return result;
}

auto DmlQueryExec::ExecuteInsertQuery(const InsertQuery &query)
    -> Result<std::vector<const Tuple*>> {
  return Error("Not implemented");
}

} // namespace JustADb
