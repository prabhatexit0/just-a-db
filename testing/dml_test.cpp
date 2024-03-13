#include <gtest/gtest.h>
#include "justadb/ddl.h"

TEST(DDLTest, DdlQueriesTest) {
  using namespace JustADb;
  const std::string DB_NAME = "test_db";

  // Use std::string instead of raw pointer for DB_NAME

  CreateDatabaseQuery create_db_query(DB_NAME);
  DatabaseManager db_manager;
  EXPECT_FALSE(db_manager.GetDatabase(DB_NAME)) << "Database already exists";

  DdlQueryExec ddl_query_exec(&db_manager);
  ddl_query_exec.ExecuteCreateDatabaseQuery(create_db_query);
  EXPECT_TRUE(db_manager.GetDatabase(DB_NAME)) << "Database not created";

  UseDatabaseQuery use_db_query(DB_NAME);
  ddl_query_exec.ExecuteUseDatabaseQuery(use_db_query);
  EXPECT_TRUE(db_manager.current_database()) << "Current database not set";
  EXPECT_EQ(DB_NAME, db_manager.current_database().value()->name()) << "Current database name mismatch";

  const std::string TABLE_NAME = "test_table";
  const std::string COLUMN_NAME = "column_1";

  // Avoid using raw pointers for Column objects. Use smart pointers (unique_ptr or shared_ptr) to manage memory.
  std::unique_ptr<Column> column_1(new Column(COLUMN_NAME, Column::Type::INT));

  CreateTableQuery create_table_query(TABLE_NAME, {column_1.get()}); // Pass pointer to column object
  const auto table_from_exec_query = ddl_query_exec.ExecuteCreateTableQuery(create_table_query);
  EXPECT_TRUE(table_from_exec_query.has_value()) << "Table not returned from executing create table query";

  const auto table_from_db_access = db_manager.current_database().value()->GetTable(TABLE_NAME);
  EXPECT_TRUE(table_from_db_access) << "Table not found in database";
  EXPECT_EQ(TABLE_NAME, table_from_db_access.value()->name()) << "Table name mismatch";

  const auto column_from_db_access = table_from_db_access.value()->GetColumn(COLUMN_NAME);
  EXPECT_TRUE(column_from_db_access) << "Column not found in table";
  EXPECT_EQ(COLUMN_NAME, column_from_db_access.value()->name()) << "Column name mismatch";

  // No need to manually delete column_1 as unique_ptr takes care of memory management.
}
