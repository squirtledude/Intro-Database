#include "db_table.hpp"
void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
    const unsigned int kRowGrowthRate = 2;
    if (col_descs_.size() == row_col_capacity_){
      // for (auto const& [key, value] : rows_) {
      //   void** cpy = new void*[row_col_capacity_ * kRowGrowthRate];
      //   for (size_t i = 0; i < col_descs_.size(); ++i){
      //     cpy[i] = value[i];
      //   }
      //   // for (size_t i = 0; i < col_descs_.size(); ++i){
      //   //   if (col_descs_.at(i).second == DataType::kInt){
      //   //     delete (static_cast<int*>(rows_[key][i]));
      //   //   } else if (col_descs_.at(i).second == DataType::kDouble){
      //   //     delete (static_cast<double*>(rows_[key][i]));
      //   //   } else if (col_descs_.at(i).second == DataType::kString){
      //   //     delete (static_cast<std::string*>(rows_[key][i]));
      //   //   }
      //   // }
      //   delete[] value;
      //   rows_.insert(std::pair<unsigned int, void**>(key, cpy));
      // }
      Resize();
      row_col_capacity_ = row_col_capacity_ * kRowGrowthRate;
    }
    col_descs_.push_back(col_desc);
    for (auto const& [key, value] : rows_) {
      if (col_desc.second == DataType::kString){
        value[col_descs_.size() - 1] = static_cast<void*>(new std::string(""));;
      } else if (col_desc.second == DataType::kInt){
        value[col_descs_.size() - 1] = static_cast<void*>(new int(0));
      } else if (col_desc.second == DataType::kDouble) {
        value[col_descs_.size() - 1] = static_cast<void*>(new double(0.0));
      }
    }
}
void DbTable::Resize() {
  void** tmp = nullptr;
  void** resize = nullptr;
  for (auto const& [key, value] : rows_){
    tmp = rows_.at(key);
    resize = new void*[static_cast<unsigned long>(row_col_capacity_ * 2)];
    for (size_t i = 0; i < row_col_capacity_; ++i){
      resize[i] = tmp[i];
    }
    delete[] tmp;
    rows_.at(key) = resize;
  }

}
void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
  if (col_idx < 0 || col_idx >= col_descs_.size()){
    throw std::out_of_range("you fucking blow");
  }
  if (col_descs_.size() == 1 && !rows_.empty()){
    throw std::runtime_error("run");
  }
  for (auto const& [key, value] : rows_) {

    if (col_descs_.at(col_idx).second == DataType::kInt){
      delete static_cast<int*>(rows_.at(key)[col_idx]);
    } else if (col_descs_.at(col_idx).second == DataType::kDouble){
      delete static_cast<double*>(rows_.at(key)[col_idx]);
    } else if (col_descs_.at(col_idx).second == DataType::kString){
      delete static_cast<std::string*>(rows_.at(key)[col_idx]);
    }
    
    for (size_t i = col_idx; i < col_descs_.size()-1; ++i){
      // if (col_descs_.at(i).second == DataType::kInt){
      //   delete (static_cast<int*>(rows_[key][i]));
      // } else if (col_descs_.at(i).second == DataType::kDouble){
      //   delete (static_cast<double*>(rows_[key][i]));
      // } else if (col_descs_.at(i).second == DataType::kString){
      //   delete (static_cast<std::string*>(rows_[key][i]));
      // }
      rows_[key][i] = rows_[key][i+1];
    }
  }
  std::vector<std::pair<std::string, DataType>>::iterator it;
  it = col_descs_.begin() + col_idx;
  col_descs_.erase(it);
}
std::vector<std::pair<std::string, DataType>> DbTable::ReturnVect(){
  return col_descs_;
}
std::map<unsigned int, void**> DbTable::ReturnMap(){
  return rows_;
}
void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
  if (col_data.size() != col_descs_.size()){
    throw std::invalid_argument("col_data is not good");
  }
  void** blank = new void*[row_col_capacity_];
  rows_.insert(std::pair<unsigned int, void**>(next_unique_id_, blank));
  next_unique_id_++;
  int count = 0;
  for (const std::string& str : col_data){
    if (col_descs_.at(count).second == DataType::kInt){
      int replace = std::stoi(str);
      rows_[next_unique_id_ - 1][count] = static_cast<void*>(new int(replace));
    } else if (col_descs_.at(count).second == DataType::kDouble){
      double replace = std::stod(str);
      rows_[next_unique_id_ - 1][count] = static_cast<void*>(new double(replace));
    } else if (col_descs_.at(count).second == DataType::kString){
      rows_[next_unique_id_ - 1][count] = static_cast<void*>(new std::string(str));
      // std::cout << str << std::endl;
    }
    count++;
  }
}
void DbTable::DeleteRowById(unsigned int id){
  if (id < 0 || id >= next_unique_id_){
    throw std::invalid_argument("incorrect id");
  }
  for (size_t i =0; i < col_descs_.size(); ++i){
    if (col_descs_.at(i).second == DataType::kInt){
      delete static_cast<int*>(rows_.at(id)[i]);
    } else if (col_descs_.at(i).second == DataType::kDouble){
      delete static_cast<double*>(rows_.at(id)[i]);
    } else if (col_descs_.at(i).second == DataType::kString){
      delete static_cast<std::string*>(rows_.at(id)[i]);
    }
  }
  delete[] rows_[id];
  rows_.erase(id);
}
DbTable::DbTable(const DbTable& rhs): next_unique_id_(rhs.next_unique_id_), row_col_capacity_(rhs.row_col_capacity_){
  col_descs_ = rhs.col_descs_;
  for (size_t i = 0; i < rhs.rows_.size(); ++i){
    void** tmp = new void*[row_col_capacity_];
    void** original = rhs.rows_.at(i);
    for (size_t j = 0; j < rhs.col_descs_.size(); ++j){
      if (rhs.col_descs_.at(j).second == DataType::kInt){
        int& inty = *(static_cast<int*>(original[j]));
        tmp[j] = static_cast<void*>(new int(inty));
      } else if (col_descs_.at(j).second == DataType::kDouble){
        double& d = *(static_cast<double*>(original[j]));
        tmp[j] = static_cast<void*>(new double(d));
      } else if (col_descs_.at(j).second == DataType::kString){
        std::string& str = *(static_cast<std::string*>(original[j]));
        tmp[j] = static_cast<void*>(new std::string(str));
      // std::cout << str << std::endl;
      }
    }
    rows_.insert(std::pair<unsigned int, void**>(i, tmp));
  }
  // for (size_t i = 0; i < rhs.col_descs_.size(); ++i){
  //   col_descs_.push_back(rhs.col_descs_.at(i));
  // }
}
DbTable::~DbTable() {
  for (unsigned int row_id = 0; row_id < next_unique_id_; ++row_id) {
    if(rows_.contains(row_id)){
      DeleteRowById(row_id);  
    }
  }
}
void DbTable::Delete() {
  for (unsigned int row_id = 0; row_id < next_unique_id_; ++row_id) {
    if(rows_.contains(row_id)){
      DeleteRowById(row_id);  
    }
  }
  // for (size_t i = 0; i < col_descs_.size(); ++i){
  //   std::vector<std::pair<std::string, DataType>>::iterator it;
  //   it = col_descs_.begin() + i;
  //   col_descs_.erase(it);
  // }
  
}
DbTable& DbTable::operator=(const DbTable& rhs){
  if (this == &rhs){
    return *this;
  }
  Delete();
  rows_.clear();
  col_descs_ = rhs.col_descs_;
  // col_descs_ = rhs.col_descs_;
  for (size_t i = 0; i < rhs.rows_.size(); ++i){
    void** tmp = new void*[rhs.row_col_capacity_];
    void** original = rhs.rows_.at(i);
    for (size_t j = 0; j < rhs.col_descs_.size(); ++j){
      if (rhs.col_descs_.at(j).second == DataType::kInt){
        int& inty = *(static_cast<int*>(original[j]));
        tmp[j] = static_cast<void*>(new int(inty));
      } else if (col_descs_.at(j).second == DataType::kDouble){
        double& d = *(static_cast<double*>(original[j]));
        tmp[j] = static_cast<void*>(new double(d));
      } else if (col_descs_.at(j).second == DataType::kString){
        std::string& str = *(static_cast<std::string*>(original[j]));
        tmp[j] = static_cast<void*>(new std::string(str));
      // std::cout << str << std::endl;
      }
    }
    rows_.insert(std::pair<unsigned int, void**>(i, tmp));
  }
  // for (size_t i = 0; i < rhs.col_descs_.size(); ++i){
  //   col_descs_.push_back(rhs.col_descs_.at(i));
  //   // std::cout << rhs.col_descs_.at(i).first << std::endl;
  //   // std::cout << rhs.col_descs_.size() << std::endl;
  // }
  row_col_capacity_ = rhs.row_col_capacity_;
  next_unique_id_ = rhs.next_unique_id_;
  // std::cout << col_descs_.size() << std::endl;
  return *this;
}
std::ostream& operator<<(std::ostream& os, const DbTable& table){
  for (size_t i = 0; i < table.col_descs_.size(); ++i){
    std::string entrytype;
    if (table.col_descs_.at(i).second == DataType::kString){
      entrytype = "std::string";
    } else if (table.col_descs_.at(i).second == DataType::kInt){
      entrytype = "int";
    } else if (table.col_descs_.at(i).second == DataType::kDouble){
      entrytype = "double";
    }
    os << table.col_descs_.at(i).first << "(" + entrytype + ")";
    if (i == table.col_descs_.size() - 1){
      os << '\n';
    } else {
      os << ", ";
    }
  }
  for (auto const& [key, value] : table.rows_){
    // std::cout << "test" << std::endl;
    for (size_t i = 0; i < table.col_descs_.size(); ++i){
      if (table.col_descs_.at(i).second == DataType::kInt){
        int& intinput = *(static_cast<int*>(table.rows_.at(key)[i]));
        os << intinput;
      } else if (table.col_descs_.at(i).second == DataType::kDouble){
        double& d = *(static_cast<double*>(table.rows_.at(key)[i]));
        os << d;
      } else if (table.col_descs_.at(i).second == DataType::kString){
        std::string& str = *(static_cast<std::string*>(table.rows_.at(key)[i]));
        os << str;
      }
      if (i == table.col_descs_.size() - 1 && key < table.rows_.size() - 1){
        os << '\n';
      } else if (i < table.col_descs_.size() - 1){
        os << ", ";
      }
    }
  }
  return os;
}