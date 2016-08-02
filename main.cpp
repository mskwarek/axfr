//
// Created by marcin on 27.07.16.
//

#include "axfrLookup.h"
#include "AxfrDatabase.hpp"

int main()
{
    axfrLookup lookup;
    lookup.performLookup("infusionsoft.com", "ns3.infusionsoft.com");
    AxfrDatabase::getInstance().addRdata();
    return 0;
}

