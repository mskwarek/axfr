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

void axfrLookup::int_parse(response_t* res)
{
    save_data_xml(res);
}

void axfrLookup::save_data_xml(response_t* res)
{
    this->database->addToLocalDb(res);
}

void axfrLookup::performLookup(const char* domain, const char* asked_ns)
{
    char *args[2] = {"onet.pl", "dns1.onet.pl"};
    tryLookup(3, args);
//    try
//    {
//      int_parse(tryLookup(domain, asked_ns));
//    }
//    catch(...)
//    {
//
//    }
}

std::vector<ScanningResult*>* axfrLookup::get_domains()
{
  if(database != nullptr)
    return this->database->get_result();
  else
    return NULL;
}
