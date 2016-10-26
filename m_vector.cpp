#include <vector>
#include <iostream>
#include <fstream>
#include <string>

extern "C" {
  std::vector<int>* new_vector(){
    return new std::vector<int>;
  }
  void delete_vector(std::vector<int>* v){
    delete v;
  }
  int vector_size(std::vector<int>* v){
    return v->size();
  }
  int vector_get(std::vector<int>* v, int i){
    return v->at(i);
  }
  void vector_push_back(std::vector<int>* v, int i){
    v->push_back(i);
  }
}
