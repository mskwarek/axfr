//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include "axfrLookup.h"


extern "C"
{
    #include "digQuery.h"
    #include <dig_parser.h>
//    void tryLookup();
};

void int_parse(response_t* res)
{
    std::cout<<res->type<<" "<<res->ttl<<" "<<res->name<<" "<<res->rdata<<std::endl;
}

void axfrLookup::performLookup()
{
    tryLookup(int_parse);
}