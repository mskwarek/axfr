//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>

#include "axfrLookup.h"

extern "C"
{
    #include "digQuery.h"
};

static void save_data(response_t* res, std::vector<std::string> tokens);
static void print_data(response_t* res, std::vector<std::string> tokens);

axfrLookup::axfrLookup()
{
    database = new AxfrDatabase();
}

axfrLookup::~axfrLookup()
{
    if(database != NULL)
        delete database;
}

void axfrLookup::int_parse(response_t* res)
{
    save_data_xml(res);
}

static void save_data(response_t* res, std::vector<std::string> tokens)
{

}
void axfrLookup::save_data_xml(response_t* res)
{
    this->database->addToLocalDb(res);
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

void axfrLookup::performLookup(const char* domain, const char* asked_ns)
{
    int_parse(tryLookup(domain, asked_ns));
    this->database->addRdata();
}