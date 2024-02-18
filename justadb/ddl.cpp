#include "ddl.h"

namespace JustADb {

auto DdlQueryExec::ExecuteCreateDatabaseQuery(
    const CreateDatabaseQuery &query) const -> Result<Database> {
  return Result<Database>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteDropDatabaseQuery(
    const DropDatabaseQuery &query) const -> Result<Database> {
  return Result<Database>(Error("Not implemented"));
}

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
