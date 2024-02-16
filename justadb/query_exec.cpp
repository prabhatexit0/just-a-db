#pragma once
#include "database.h"
#include "dml.h"

namespace JustADb {
class QueryExec {
public:
  QueryExec(Database db) : db_(db) {}

  // TODO: implement methods for query excution.

private:
  Database db_;
};
} // namespace JustADb
