#include "ddl.h"
#include <iostream>

int main() {
  JustADb::DatabaseManager db_manager;
  JustADb::DdlQueryExec query_exec(&db_manager);
  JustADb::CreateDatabaseQuery create_db_query("test_db");

  auto db_res = query_exec.ExecuteCreateDatabaseQuery(create_db_query);
  if (!db_res) {
    std::cerr << "Error: " << db_res.error().message() << std::endl;
    return 1;
  }

  std::cout << "Database created: " << create_db_query.db_name() << std::endl;

  return 0;
}
