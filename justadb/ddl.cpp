#include "ddl.h"

namespace JustADb {

auto DdlQueryExec::ExecuteCreateTableQuery(const CreateTableQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteDropTableQuery(const DropTableQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteAlterTableQuery(const AlterTableQuery &query) const
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

} // namespace JustADb
