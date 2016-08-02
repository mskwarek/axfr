//
// Created by marcin on 27.07.16.
//

#include "axfrLookup.h"
#include "AxfrDatabase.hpp"

int main()
{
//    AxfrDatabase::getInstance().initXmlFile();
    axfrLookup lookup;
    lookup.performLookup();
    AxfrDatabase::getInstance().addRdata();
    return 0;
}

