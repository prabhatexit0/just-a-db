#include "ddl.h"

#include <utility>

namespace JustADb {

auto Tuple::GetValue(const std::string& column_name) const
    -> std::optional<const Value *> {
  auto value = value_map_.find(column_name);
  if (value != value_map_.end()) {
    return value->second;
  }
  return std::nullopt;
}

void Tuple::InsertValue(const std::string& column_name, const Value *value) {
  value_map_.insert({column_name, value});
}

  auto Table::InsertTuple(Tuple *tuple) -> Result<const Tuple *> {
    if (tuple->values_map().size() != columns_.size()) {
      return Error("Tuple does not match table schema");
    }

    tuples_.push_back(tuple);
    return tuple;
  }

  auto Table::SelectTuple(const std::string& column_name, const Value *value) const
      -> std::vector<const Tuple *> {
    std::vector<const Tuple *> result;
    for (const auto &tuple : tuples_) {
      if (auto tuple_value = tuple->GetValue(column_name);
          tuple_value.has_value() && tuple_value.value() == value) {
        result.push_back(tuple);
      }
    }
    return result;
  }

  auto Table::AddColumn(Column *column) -> Result<const Table *> {
    for (const auto &col : columns_) {
      if (col->name() == column->name()) {
        return Error("Column already exists");
      }
    }

    columns_.push_back(column);
    return this;
  }

  auto Table::DropColumn(const std::string& column_name) -> Result<const Table*> {
    for (auto it = columns_.begin(); it != columns_.end(); ++it) {
      if ((*it)->name() == column_name) {
        columns_.erase(it);
        return this;
      }
    }

    return Error("Column does not exist");
  }

auto Database::CreateTable(const std::string& name,
                           std::vector<Column *> columns)
    -> Result<const Table *> {
  if (tables_.find(name) != tables_.end()) {
    return Error("Table already exists");
  }
  auto *table = new Table(name, std::move(columns));
  auto [it, is_inserted] = tables_.insert({table->name(), table});
  if (!is_inserted) {
    return Error("Cannot create table");
  }
  return it->second;
}

auto Database::DropTable(const std::string& table_name) -> Result<bool> {
  if (tables_.find(table_name) == tables_.end()) {
    return Error("Table does not exist");
  }

  // Find a way to know if table is really dropped
  tables_.erase(table_name);
  return true;
}

auto Database::UpdateTable(const std::string& table_name, Table *table)
    -> Result<const Table *> {
  if (tables_.find(table_name) == tables_.end()) {
    return Error("Table does not exist");
  }

  tables_.erase(table_name);
  tables_[table_name] = table;
  return table;
}

auto Database::GetTable(const std::string& name) const
    -> std::optional<const Table *> {
  if (auto something = tables_.find(name); something != tables_.end()) {
    return something->second;
  }

  return std::nullopt;
}

auto Database::GetModifiableTable(const std::string &name) const -> std::optional<Table *> {
    if (auto something = tables_.find(name); something != tables_.end()) {
        return something->second;
    }

    return std::nullopt;
}


auto DatabaseManager::CreateDatabase(const std::string& name)
    -> Result<const Database *> {
  if (databases_.find(name) != databases_.end()) {
    return Error("Database already exists");
  }
  auto *db = new Database(name);
  databases_.insert({name, db});
  return db;
}

auto DatabaseManager::DropDatabase(const std::string& name) -> Result<bool> {
  if (databases_.find(name) == databases_.end()) {
    return Error("Database does not exist");
  }

  return databases_.erase(name);
}

auto DatabaseManager::GetDatabase(const std::string& name) const
    -> std::optional<Database *> {
  if (auto something = databases_.find(name); something != databases_.end()) {
    return something->second;
  }

  return std::nullopt;
}

auto DdlQueryExec::ExecuteUseDatabaseQuery(const UseDatabaseQuery &query)
    -> Result<bool> {
  auto db = db_manager_->GetDatabase(query.db_name());
  if (!db.has_value()) {
    return Error("Database does not exist");
  }
  db_manager_->SetCurrentDatabase(*db);
  return true;
}

auto DdlQueryExec::ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
    -> Result<const Database *> {
  if (query.db_name().empty()) {
    return Error("Database name cannot be empty");
  }
  return db_manager_->CreateDatabase(query.db_name());
}

auto DdlQueryExec::ExecuteDropDatabaseQuery(const DropDatabaseQuery &query)
    -> Result<bool> {
  return db_manager_->DropDatabase(query.db_name());
}

auto DdlQueryExec::ExecuteCreateTableQuery(const CreateTableQuery &query)
    -> Result<const Table *> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return Error("No database selected");
  }
  return (*current_db)->CreateTable(query.table_name(), query.columns());
}

auto DdlQueryExec::ExecuteDropTableQuery(const DropTableQuery &query)
    -> Result<bool> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return Error("No database selected");
  }
  return (*current_db)->DropTable(query.table_name());
}

auto DdlQueryExec::ExecuteAlterTableQuery(const AlterTableQuery &query)
    -> Result<const Table *> {
    auto current_db = db_manager_->current_database();
    if (!current_db) {
        return Error("No database selected");
    }

    auto table = (*current_db)->GetModifiableTable(query.table_name());
    if (!table) {
        return Error("Table name not found");
    }

    switch(query.alter_type()) {
        case AlterTableQuery::AlterType::ADD_COLUMN: {
            return (*table)->AddColumn(query.column());
        }
        case AlterTableQuery::AlterType::DROP_COLUMN: {
            return (*table)->DropColumn(query.column()->name());
        }
        case AlterTableQuery::AlterType::MODIFY_DATATYPE: {
            for (auto* column : (*table)->columns()) {
                if (column->name() == query.column()->name())  {
                    column->set_type(query.column()->type());
                    return *table;
                }
            }

            return Error("Column not found");
        }
        case AlterTableQuery::AlterType::RENAME_COLUMN: {
            for (auto* column : (*table)->columns()) {
                if (column->name() == query.column()->name())  {
                    column->rename_column(query.column()->name());
                    return *table;
                }
            }

            return Error("Column not found");
        }
    }
}

} // namespace JustADb
