#include <optional>
#include <string>
#include <vector>
#include "dml.h"


int main() {
  Query q(Query::Kind::SELECT, "table", {"column1", "column2"});
  q.Where("column1", "value", WhereClause::Operator::EQUAL)
      .OrderBy("column1", OrderByClause::Order::ASCENDING);
  return 0;
}
