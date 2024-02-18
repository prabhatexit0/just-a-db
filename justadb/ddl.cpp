#include "ddl.h"

namespace JustADb {

auto DdlQueryExec::ExecuteCreateTableQuery(const DdlQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteDropTableQuery(const DdlQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteAlterTableQuery(const DdlQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

} // namespace JustADb
