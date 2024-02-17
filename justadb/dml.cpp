#include "dml.h"

namespace JustADb {

auto DmlQueryExec::ExecuteSelectQuery(const SelectQuery &query)
    -> Result<std::vector<Tuple>> {
  if (query.columns().empty()) {
    return Result<std::vector<Tuple>>(Error("No columns are selected"));
  }

  std::optional<Table> table = this->db_.get_table(query.table().name());
  if (!table.has_value()) {
    return Result<std::vector<Tuple>>(Error("Table not found"));
  }

  std::vector<Tuple> result;
  for (const auto &tuple : (*table).tuples()) {

    Tuple new_tuple;
    for (const auto &column : query.columns()) {
      auto value = tuple.get_value(column.name());
      if (value.has_value()) {
        new_tuple.insert_value(column.name(), *value);
      }
    }

    if (new_tuple.is_empty()) {
      return Result<std::vector<Tuple>>(Error("No column found"));
    }
  }

  return result;
}

} // namespace JustADb
