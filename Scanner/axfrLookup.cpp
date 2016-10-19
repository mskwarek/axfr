//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
extern "C"
{
#include "../digUtils/digQuery.h"
}
#include <dig_parser.h>

#include "axfrLookup.h"

axfrLookup::axfrLookup()
{
    database = new AxfrDatabase();
}

axfrLookup::~axfrLookup()
{
    if(database != NULL)
        delete database;
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void axfrLookup::int_parse(response_t* res)
{
    save_data_xml(res);
}

void axfrLookup::int_parse(char* res)
{
  if(res == NULL)
    {
      return;
    }
  this->response = std::string(res);
    std::istringstream iss(this->response);
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(iss, line))
    {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter(tokens));
        try
        {
            ScanningResult *res = new ScanningResult(this->name_scanned, tokens[0], tokens[1], tokens[2], tokens[3], tokens[4]);
            this->database->add_to_db(res);
        }
        catch(...)
        { }
    }
}

void axfrLookup::print()
{
//    std::cout<<this->response<<std::endl;
}

void axfrLookup::save_data_xml(response_t* res)
{
    this->database->addToLocalDb(res);
}

void axfrLookup::performLookup(const char* domain, const char* asked_ns)
{
    char* tmp = NULL;
    this->name_scanned = std::string(domain);
    tmp = new char(strlen(asked_ns) + 1);
    memmove(tmp+1, asked_ns, strlen(asked_ns));
    tmp[0] = '@';
    std::string nameserver(tmp, strnlen(tmp, 100));
    trim(nameserver);
    const char *args[4] = {"axfrscan", domain, nameserver.c_str(), "axfr"};
    int_parse(tryLookup(4, (char**) args));
    delete tmp;
}

std::vector<ScanningResult*>* axfrLookup::get_domains()
{
  if(database != nullptr)
    return this->database->get_result();
  else
    return NULL;
}

