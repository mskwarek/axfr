//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>


#include "axfrLookup.h"
#include "AxfrDatabase.hpp"


extern "C"
{
    #include "digQuery.h"
};

static void save_data(response_t* res, std::vector<std::string> tokens);
static void save_data_xml(response_t* res, std::vector<std::string> tokens);
static void print_data(response_t* res, std::vector<std::string> tokens);

void int_parse(response_t* res)
{
    std::vector<std::string> tokens;

    std::string sentence(res->rdata);
    std::istringstream iss(sentence);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         back_inserter(tokens));

    save_data_xml(res, tokens);

}

static void save_data(response_t* res, std::vector<std::string> tokens)
{

}
static void save_data_xml(response_t* res, std::vector<std::string> tokens)
{
    AxfrDatabase::getInstance().addToLocalDb(*res, tokens);
}

static void print_data(response_t* res, std::vector<std::string> tokens)
{
    for (auto const &n : tokens)
    {
        if(!(res->cls == "SOA"))
            std::cout << ' ' << n << ' ' << res->cls << ' ' << res->type;
    }
    std::cout<<std::endl;
}

void axfrLookup::performLookup()
{
    tryLookup(int_parse);
}