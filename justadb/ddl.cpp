#include "ddl.h"
#include <iostream>

namespace JustADb {

auto DdlQueryExec::ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
    -> Result<const Database*> {
  if (query.db_name() == "") {
    return Error("Database name cannot be empty");
  }
  return db_manager_->create_database(query.db_name());
}

auto DdlQueryExec::ExecuteDropDatabaseQuery(const DropDatabaseQuery &query)
    -> Result<bool> {
  return db_manager_->drop_database(query.db_name());
}

auto DdlQueryExec::ExecuteCreateTableQuery(const CreateTableQuery &query)
    ->  Result<const Table*> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return Error("No database selected");
  }
  return (*current_db)->create_table(query.table_name(), query.columns());

}

auto DdlQueryExec::ExecuteDropTableQuery(const DropTableQuery &query)
    -> Result<bool> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return Error("No database selected");
  }
  return (*current_db)->drop_table(query.table_name());
}

auto DdlQueryExec::ExecuteAlterTableQuery(const AlterTableQuery &query)
    -> Result<const Table*> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return Error("No database selected");
  }

  auto table = (*current_db)->get_table(query.table_name());
  if (!table) {
    return Error("Table does not exist");
  }
  return Result<const Table*>(Error("Not implemented"));
}

} // namespace JustADb
