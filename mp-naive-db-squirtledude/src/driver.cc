#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

int main() {
  // write informal tests here
  DbTable test = DbTable();
  std::pair<std::string, DataType> PAIR2("GeeksForGeeks", DataType::kString);
  std::pair<std::string, DataType> PAIR1("num1", DataType::kInt);
  std::pair<std::string, DataType> PAIR3("num2", DataType::kInt);
  std::pair<std::string, DataType> PAIR4("128", DataType::kInt);
  std::pair<std::string, DataType> PAIR5("173", DataType::kInt);

  test.AddColumn(PAIR2);
  test.AddColumn(PAIR1);
  test.AddColumn(PAIR3);
  
  DbTable threetest = DbTable();
  threetest.AddColumn(PAIR4);
  threetest.AddColumn(PAIR5);
  threetest.AddRow({"155", "76"});
  threetest.AddRow({"153", "70"});

  test.AddRow({"hello", "12", "44"});
  test.AddRow({"why", "22", "1909"});
  test.AddRow({"for", "222", "183"});
  // test.Delete();
  std::cout << test << std::endl;
  // test.DeleteColumnByIdx(1);
  // test.DeleteColumnByIdx(1);
  // std::cout << test.ReturnVect().size() << std::endl;
  // for (size_t i = 0; i < test.ReturnVect().size(); ++i){
    // std::cout << test.ReturnVect().at(i).first << std::endl;
  // }
  // std::cout << test.ReturnMap().at(0).size() << std::endl;
  // std::string& str = *(static_cast<std::string*>(test.ReturnMap().at(0)[0]));
  // int& i = *(static_cast<int*>(test.ReturnMap().at(1)[1]));
  // std::cout << str << std::endl; 
  // std::cout << i << std::endl;
  // test.AddColumn(PAIR4);
  // std::cout << test.ReturnVect().size() << std::endl;
  // std::cout << test.ReturnVect().size() << std::endl;
  // test.DeleteColumnByIdx(1);
  // std::cout << test.ReturnVect().size() << std::endl;
  // std::cout << test.ReturnVect().at(0).first << std::endl;
  // int& iagain = *(static_cast<int*>(test.ReturnMap().at(0)[1]));
  // std::cout << iagain << std::endl;

  // std::cout << test << std::endl;
  // DbTable twotest = DbTable(test);
  // std::cout << twotest << std::endl;
  // std::cout << twotest.ReturnVect().at(1).first << std::endl;
  threetest = test;
  std::cout << test << std::endl;
  std::cout << threetest << std::endl;
  // test = test;
  // std::cout << test << std::endl;
  // DbTable bigtest = DbTable(test);
  // std::cout << bigtest << std::endl;
  // std::cout << test.ReturnVect().size() << std::endl;
  // std::cout << test.ReturnId() << std::endl;
}