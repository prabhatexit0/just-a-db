#pragma once
#include "ddl.h"
#include <optional>
#include <string>

namespace JustADb {
// This would need to have something to handle connections and something to
// execute a query for now.
// also, first of all - it should have a name :p
// maybe an unordered list of tables?
class Database {
public:
  Database(std::string name) : name_(name) {}

  void create_table(std::string name, Table table) {
    tables_.insert({name, table});
  }

  void drop_table(std::string name) { tables_.erase(name); }

  auto get_table(std::string name) const -> std::optional<Table> {
    if (auto something = tables_.find(name); something != tables_.end()) {
      return something->second;
    }

    return std::nullopt;
  }

  auto name() const { return name_; }

private:
  std::string name_;
  std::unordered_map<std::string, Table> tables_;
};

} // namespace JustADb
