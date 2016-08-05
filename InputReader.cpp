//
// Created by marcin on 02.08.16.
//

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include "InputReader.h"

InputReader::InputReader() { }

void InputReader::ReadFromFile(std::string path)
{
    std::ifstream dataFile(path);

    if (dataFile.is_open())
    {
        try_to_process(&dataFile);
    }
}

void InputReader::try_to_process(std::ifstream* dataFile)
{
    try
    {
        process_file(dataFile);
    }
    catch(...)
    {
    }

}

void InputReader::process_file(std::ifstream* dataFile)
{
    std::string line = "";

    while (std::getline(*dataFile, line) )
    {
        std::vector<std::string> temp;
        std::istringstream iss(line);
        std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                back_inserter(temp));
        data.push_back(ScannedDomain(temp.at(0).c_str(), temp.at(1).c_str()));
    }
    dataFile->close();
}

std::vector<ScannedDomain> InputReader::GetData()
{
    return data;
}