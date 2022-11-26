#include "db.hpp"
void Database::CreateTable(const std::string& table_name) {
  auto* table = new DbTable();
  tables_.insert({table_name, table});
}
void Database::DropTable(const std::string& table_name){
    if (!tables_.contains(table_name)){
        throw std::invalid_argument("");
    }

    // auto tmp = *tables_.at(table_name);
    delete tables_.at(table_name);
    tables_.erase(table_name);
    
    // tmp->Delete();
    // tables_.at(table_name) = nullptr;
    
    // delete tables_.at(table_name);
    
}
DbTable& Database::GetTable(const std::string& table_name){
    return *tables_[table_name];
}
Database::Database(const Database& rhs) {
    for (auto const& [key, value] : rhs.tables_){
      DbTable ptr = *value;
      auto* ptrr = new DbTable(ptr);
      tables_.insert(std::pair<std::string, DbTable*>(key, ptrr));
    }
}
Database& Database::operator=(const Database& rhs) {
  if (this == &rhs){
    return *this;
  }
  Delete();
  for (auto const& [key, value] : rhs.tables_){
    DbTable ptr = *value;
    auto* ptrr = new DbTable(ptr);
    tables_.insert(std::pair<std::string, DbTable*>(key, ptrr));
  }
  return *this;
}
Database::~Database() {
  for (auto const& [key, value] : tables_) {
    value->Delete();
    delete tables_[key];
    // tables_.erase(key);
  }
  tables_.clear();
}
void Database::Delete() {
  for (auto const& [key, value] : tables_) {
    value->Delete();
    delete tables_[key];
  }
  tables_.clear();
}