//
// Created by marcin on 02.08.16.
//

#ifndef MYDIG_INPUTREADER_H
#define MYDIG_INPUTREADER_H

#include <string>
#include <vector>
#include "ScannedDomain.hpp"

class InputReader {
public:
    InputReader();
    void ReadFromFile(std::string path);
    std::vector<ScannedDomain> GetData();

private:
    std::vector<ScannedDomain> data;
};


#endif //MYDIG_INPUTREADER_H
