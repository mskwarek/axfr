#include <iostream>
#include <fstream>

extern "C"{
#include"dns.h"
}

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdlib.h>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}


std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}


int main(int argc, char* argv[]){
  std::string line;
  int i=0;

  if(argc>=4)
  {
    std::cout<<"Main args: "<<argv[1]<<" " <<argv[2]<<std::endl;
    ngethostbyname(argv[1], argv[2], argv[5], atoi(argv[3]), atoi(argv[4]));
  }
  else
  {
  std::string filename = argv[1];
  std::ifstream inputFile(filename.c_str());
  while(std::getline(inputFile, line)){
    std::cout<<i++<<std::endl;
    std::vector<std::string> x = split(line, ' ');
    ngethostbyname(x[0].c_str(), x[1].c_str(), argv[3], 252, atoi(argv[2]));
  }
  }
  return 0;
}
