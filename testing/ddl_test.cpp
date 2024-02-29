#include <gtest/gtest.h>
#include "justadb/ddl.h"

TEST(DDLTest, DdlQueriesTest) {
    auto db_name = "test_db";
    JustADb::CreateDatabaseQuery create_db_query("test_db");
    JustADb::DatabaseManager db_manager;
    EXPECT_FALSE(db_manager.GetDatabase(db_name)) << "Database already exists";

    JustADb::DdlQueryExec ddl_query_exec(&db_manager);
    ddl_query_exec.ExecuteCreateDatabaseQuery(create_db_query);
    EXPECT_TRUE(db_manager.GetDatabase(db_name)) << "Database not created";

    JustADb::UseDatabaseQuery use_db_query("test_db");
}
