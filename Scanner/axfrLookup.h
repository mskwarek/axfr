//
// Created by marcin on 27.07.16.
//

#ifndef MYDIG_AXFRLOOKUP_H
#define MYDIG_AXFRLOOKUP_H

#include "AxfrDatabase.hpp"

extern "C"
{
#include <dig_parser.h>
#include "../digUtils/digQuery.h"
};

class axfrLookup {
public:
    axfrLookup();
    ~axfrLookup();
    void performLookup(const char* domain, const char* asked_ns);
    std::vector<ScanningResult*>* get_domains();
private:
    AxfrDatabase* database;
    void save_data_xml(response_t* res);
    void int_parse(response_t* res);
};


#endif //MYDIG_AXFRLOOKUP_H
