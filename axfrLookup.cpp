//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include "axfrLookup.h"


extern "C"
{
    #include "digQuery.h"
//    void tryLookup();
};

void axfrLookup::performLookup()
{

    tryLookup();
    std::cout<<getResponse();
}