#include "ddl.h"
#include <iostream>

namespace JustADb {

auto DdlQueryExec::ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
    -> Result<Database> {
  if (query.db_name() == "") {
    return Result<Database>(Error("Database name cannot be empty"));
  }
  return db_manager_.create_database(query.db_name());
}

auto DdlQueryExec::ExecuteDropDatabaseQuery(const DropDatabaseQuery &query)
    -> Result<Database> {
  return Result<Database>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteCreateTableQuery(const CreateTableQuery &query)
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteDropTableQuery(const DropTableQuery &query)
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

auto DdlQueryExec::ExecuteAlterTableQuery(const AlterTableQuery &query)
    -> Result<Table> {
  return Result<Table>(Error("Not implemented"));
}

} // namespace JustADb
