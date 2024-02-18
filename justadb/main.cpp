#include "ddl.h"
#include "dml.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>

int main() {
  JustADb::DatabaseManager db_manager;
  JustADb::DdlQueryExec query_exec(db_manager);
  JustADb::CreateDatabaseQuery create_db_query("test_db");

  auto db_res = query_exec.ExecuteCreateDatabaseQuery(create_db_query);
  if (db_res.error()) {
    std::cerr << "Error: " << db_res.error().value().message() << std::endl;
    return 1;
  }

  std::cout << "Database created: " << db_res.value().name() << std::endl;

  return 0;
}
