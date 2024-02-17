#include "dml.h"

namespace JustADb {

auto DmlQueryExec::ExecuteSelectQuery(const SelectQuery &query)
    -> std::optional<std::vector<Tuple>> {
  if (query.columns().empty()) {
    return std::nullopt;
  }

  std::optional<Table> table = this->db_.get_table(query.table().name());
  if (!table.has_value()) {
    return std::nullopt;
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
      // TODO: reutrn error ["No column found"]
    }
  }

  return result;
}

} // namespace JustADb
