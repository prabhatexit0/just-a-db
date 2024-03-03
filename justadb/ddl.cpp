#include "ddl.h"

namespace JustADb {

auto Tuple::GetValue(const std::string &column_name) const
    -> std::optional<const Value *> {
  auto value = value_map_.find(column_name);
  if (value != value_map_.end()) {
    return value->second;
  }
  return std::nullopt;
}

void Tuple::InsertValue(const std::string &column_name, const Value *value) {
  value_map_.insert({column_name, value});
}

auto Table::InsertTuple(Tuple *tuple) -> std::expected<const Tuple *, Error> {
  if (tuple->values_map().size() != columns_.size()) {
    return std::unexpected(Error("Tuple does not match table schema"));
  }

  tuples_.push_back(tuple);
  return tuple;
}

auto Table::SelectTuple(const std::string &column_name,
                        const Value *value) const
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

auto Table::AddColumn(Column *column) -> std::expected<const Table *, Error> {
  for (const auto &col : columns_) {
    if (col->name() == column->name()) {
      return std::unexpected(Error("Column already exists"));
    }
  }

  columns_.push_back(column);
  return this;
}

auto Table::DropColumn(const std::string &column_name)
    -> std::expected<const Table *, Error> {
  for (auto it = columns_.begin(); it != columns_.end(); ++it) {
    if ((*it)->name() == column_name) {
      columns_.erase(it);
      return this;
    }
  }

  return std::unexpected(Error("Column does not exist"));
}

auto Database::CreateTable(const std::string &name,
                           std::vector<Column *> columns)
    -> std::expected<const Table *, Error> {
  if (tables_.find(name) != tables_.end()) {
    return std::unexpected(Error("Table already exists"));
  }
  auto *table = new Table(name, std::move(columns));
  auto [it, is_inserted] = tables_.insert({table->name(), table});
  if (!is_inserted) {
    return std::unexpected(Error("Cannot create table"));
  }
  return it->second;
}

auto Database::DropTable(const std::string &table_name) -> std::expected<void, Error> {
  if (tables_.find(table_name) == tables_.end()) {
    return std::unexpected(Error("Table does not exist"));
  }

  tables_.erase(table_name);
  return std::expected<void, Error>(std::in_place);
}

auto Database::UpdateTable(const std::string &table_name, Table *table)
    -> std::expected<const Table *, Error> {
  if (tables_.find(table_name) == tables_.end()) {
    return std::unexpected(Error("Table does not exist"));
  }

  tables_.erase(table_name);
  tables_[table_name] = table;
  return table;
}

auto Database::GetTable(const std::string &name) const
    -> std::optional<const Table *> {
  if (auto something = tables_.find(name); something != tables_.end()) {
    return something->second;
  }

  return std::nullopt;
}

auto Database::GetModifiableTable(const std::string &name) const
    -> std::optional<Table *> {
  if (auto something = tables_.find(name); something != tables_.end()) {
    return something->second;
  }

  return std::nullopt;
}

auto DatabaseManager::CreateDatabase(const std::string &name)
    -> std::expected<void, Error> {
  if (databases_.find(name) != databases_.end()) {
    return std::unexpected(Error("Database already exists"));
  }
  auto *db = new Database(name);
  databases_.insert({name, db});
  return std::expected<void, Error>(std::in_place);
}

auto DatabaseManager::DropDatabase(const std::string &name) -> std::expected<void, Error> {
  if (databases_.find(name) == databases_.end()) {
    return std::unexpected(Error("Database does not exist"));
  }

  databases_.erase(name);
  return std::expected<void, Error>(std::in_place);
}

auto DatabaseManager::GetDatabase(const std::string &name) const
    -> std::optional<Database *> {
  if (auto something = databases_.find(name); something != databases_.end()) {
    return something->second;
  }

  return std::nullopt;
}

auto DdlQueryExec::ExecuteUseDatabaseQuery(const UseDatabaseQuery &query)
    -> std::expected<void, Error> {
  auto db = db_manager_->GetDatabase(query.db_name());
  if (!db.has_value()) {
    return std::unexpected(Error("Database does not exist"));
  }
  return db_manager_->SetCurrentDatabase(*db);
}

auto DdlQueryExec::ExecuteCreateDatabaseQuery(const CreateDatabaseQuery &query)
    -> std::expected<void, Error> {
  if (query.db_name().empty()) {
    return std::unexpected(Error("Database name cannot be empty"));
  }
  return db_manager_->CreateDatabase(query.db_name());
}

auto DdlQueryExec::ExecuteDropDatabaseQuery(const DropDatabaseQuery &query)
    -> std::expected<void, Error> {
  return db_manager_->DropDatabase(query.db_name());
}

auto DdlQueryExec::ExecuteCreateTableQuery(const CreateTableQuery &query)
    -> std::expected<const Table *, Error> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return std::unexpected(Error("No database selected"));
  }
  return (*current_db)->CreateTable(query.table_name(), query.columns());
}

auto DdlQueryExec::ExecuteDropTableQuery(const DropTableQuery &query)
    -> std::expected<void, Error> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return std::unexpected(Error("No database selected"));
  }
  return (*current_db)->DropTable(query.table_name());
}

auto DdlQueryExec::ExecuteAlterTableQuery(const AlterTableQuery &query)
    -> std::expected<const Table *, Error> {
  auto current_db = db_manager_->current_database();
  if (!current_db) {
    return std::unexpected(Error("No database selected"));
  }

  auto table = (*current_db)->GetModifiableTable(query.table_name());
  if (!table) {
    return std::unexpected(Error("Table name not found"));
  }

  switch (query.alter_type()) {
  case AlterTableQuery::AlterType::ADD_COLUMN: {
    return (*table)->AddColumn(query.column());
  }
  case AlterTableQuery::AlterType::DROP_COLUMN: {
    return (*table)->DropColumn(query.column()->name());
  }
  case AlterTableQuery::AlterType::MODIFY_DATATYPE: {
    for (auto *column : (*table)->columns()) {
      if (column->name() == query.column()->name()) {
        column->set_type(query.column()->type());
        return *table;
      }
    }

    return std::unexpected(Error("Column not found"));
  }
  case AlterTableQuery::AlterType::RENAME_COLUMN: {
    for (auto *column : (*table)->columns()) {
      if (column->name() == query.column()->name()) {
        column->rename_column(query.column()->name());
        return *table;
      }
    }

    return std::unexpected(Error("Column not found"));
  }
  }
}

} // namespace JustADb
