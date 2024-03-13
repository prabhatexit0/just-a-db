#include "justadb/ddl.h"
#include <gtest/gtest.h>

TEST(DDLTest, DdlQueriesTest) {
  using namespace JustADb;
  const auto DB_NAME = "test_db";

  // Create Database Query test
  CreateDatabaseQuery create_db_query(DB_NAME);
  DatabaseManager db_manager;
  EXPECT_FALSE(db_manager.GetDatabase(DB_NAME)) << "Database already exists";

  DdlQueryExec ddl_query_exec(&db_manager);
  auto query_exec_result =
      ddl_query_exec.ExecuteCreateDatabaseQuery(create_db_query);
  EXPECT_TRUE(db_manager.GetDatabase(DB_NAME)) << "Database not created";
  EXPECT_TRUE(query_exec_result) << "Error while executing Create DB Query";

  // Get it in a variable to reference it easily :)
  const auto DB = db_manager.GetDatabase(DB_NAME).value();

  // User Database Query test
  UseDatabaseQuery use_db_query(DB_NAME);
  query_exec_result = ddl_query_exec.ExecuteUseDatabaseQuery(use_db_query);
  EXPECT_TRUE(query_exec_result) << "Error while executing use database query";
  EXPECT_TRUE(db_manager.current_database()) << "Current database not set";
  EXPECT_EQ(DB_NAME, db_manager.current_database().value()->name())
      << "Current database name mismatch";

  const auto TABLE_NAME = "test_table";
  const auto COLUMN_NAME = "column_1";

  // Create Table Query test
  auto *column_1 = new Column(COLUMN_NAME, Column::Type::INT);
  CreateTableQuery create_table_query(TABLE_NAME, {column_1});
  const auto create_table_exec_result =
      ddl_query_exec.ExecuteCreateTableQuery(create_table_query);
  EXPECT_TRUE(query_exec_result) << "Error while executing create table query";
  EXPECT_TRUE(create_table_exec_result.has_value())
      << "Table not returned from executing create table query";

  const auto table_from_db_access = DB->GetTable(TABLE_NAME);
  EXPECT_TRUE(table_from_db_access) << "Table not found in database";

  // Add it in a variable for easy access
  const auto TABLE = table_from_db_access.value();
  EXPECT_EQ(TABLE_NAME, TABLE->name()) << "Table name mismatch";

  const auto column_from_db_access = TABLE->GetColumn(COLUMN_NAME);
  EXPECT_TRUE(column_from_db_access) << "Column not found in table";
  EXPECT_EQ(COLUMN_NAME, column_from_db_access.value()->name())
      << "Column name mismatch";

  // Alter Table Query Test
  // Add Column
  const auto COLUMN_NAME_0 = "column_name_0";
  auto *column_2 = new Column(COLUMN_NAME_0, Column::Type::INT);
  AlterTableQuery add_column_query(
      TABLE_NAME, AlterTableQuery::AlterType::ADD_COLUMN, column_2);
  const auto add_col_exec_result =
      ddl_query_exec.ExecuteAlterTableQuery(add_column_query);
  EXPECT_TRUE(add_col_exec_result) << "Error while executing add column query";
  EXPECT_TRUE(TABLE->GetColumn(COLUMN_NAME_0)) << "Column not added";
  EXPECT_EQ(COLUMN_NAME_0, TABLE->GetColumn(COLUMN_NAME_0).value()->name())
      << "Column name mismatch";

  // Drop Column
  AlterTableQuery drop_column_query(
      TABLE_NAME, AlterTableQuery::AlterType::DROP_COLUMN, column_2);
  const auto drop_col_exec_result =
      ddl_query_exec.ExecuteAlterTableQuery(drop_column_query);
  EXPECT_TRUE(drop_col_exec_result) << "Error while executing drop column query"
                                    << drop_col_exec_result.error().message();
  EXPECT_FALSE(TABLE->GetColumn(COLUMN_NAME_0)) << "Column not dropped";

  // Rename Column
  const std::string NEW_COLUMN_NAME = "new_column_name";
  AlterTableQuery rename_column_query(TABLE_NAME,
                                      AlterTableQuery::AlterType::RENAME_COLUMN,
                                      column_1, NEW_COLUMN_NAME);
  const auto rename_col_exec_result =
      ddl_query_exec.ExecuteAlterTableQuery(rename_column_query);
  EXPECT_TRUE(rename_col_exec_result)
      << "Error while executing rename column query: "
      << rename_col_exec_result.error().message();
  EXPECT_EQ(NEW_COLUMN_NAME, TABLE->GetColumn(NEW_COLUMN_NAME).value()->name())
      << "Column name mismatch";

  // Modify datatype
  const auto NEW_COLUMN_TYPE = Column::Type::FLOAT;
  AlterTableQuery modify_datatype_query(
      TABLE_NAME, AlterTableQuery::AlterType::MODIFY_DATATYPE, column_1,
      std::nullopt, NEW_COLUMN_TYPE);
  const auto modify_datatype_exec_result =
      ddl_query_exec.ExecuteAlterTableQuery(modify_datatype_query);
  EXPECT_TRUE(modify_datatype_exec_result)
      << "Error while executing modify datatype query: "
      << modify_datatype_exec_result.error().message();
  EXPECT_EQ(NEW_COLUMN_TYPE, TABLE->GetColumn(NEW_COLUMN_NAME).value()->type())
      << "Column type mismatch";

  // Drop Table Query test
  DropTableQuery drop_table_query(TABLE_NAME);
  const auto drop_table_exec_result =
      ddl_query_exec.ExecuteDropTableQuery(drop_table_query);
  EXPECT_TRUE(drop_table_exec_result)
      << "Error while executing drop table query";
  EXPECT_FALSE(DB->GetTable(TABLE_NAME)) << "Table not dropped";

  // Drop Database Query test
  DropDatabaseQuery drop_db_query(DB_NAME);
  const auto drop_db_exec_result =
      ddl_query_exec.ExecuteDropDatabaseQuery(drop_db_query);
  EXPECT_TRUE(drop_db_exec_result) << "Error while executing drop db query: " << drop_db_exec_result.error().message();
  EXPECT_FALSE(db_manager.GetDatabase(DB_NAME)) << "Database not dropped";
}
