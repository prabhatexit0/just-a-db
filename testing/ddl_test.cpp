#include <gtest/gtest.h>
#include "justadb/ddl.h"

TEST(DDLTest, DdlQueriesTest) {
    using namespace JustADb;
    const auto DB_NAME = "test_db";

CreateDatabaseQuery create_db_query(DB_NAME);
    DatabaseManager db_manager;
    EXPECT_FALSE(db_manager.GetDatabase(DB_NAME)) << "Database already exists";

    DdlQueryExec ddl_query_exec(&db_manager);
    ddl_query_exec.ExecuteCreateDatabaseQuery(create_db_query);
    EXPECT_TRUE(db_manager.GetDatabase(DB_NAME)) << "Database not created";

    UseDatabaseQuery use_db_query(DB_NAME);
    EXPECT_TRUE(db_manager.current_database().value()->name() == DB_NAME)  << "Database not set";
}
