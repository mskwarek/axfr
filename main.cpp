//
// Created by marcin on 27.07.16.
//

#include "axfrLookup.h"
#include "AxfrDatabase.hpp"
#include "InputReader.h"

int main()
{
    axfrLookup lookup;
    InputReader reader;
    reader.ReadFromFile("/home/marcin/mgr/test");

    for(auto i : reader.GetData())
    {
        lookup.performLookup(i.GetDomainAddress().c_str(), i.GetNsAddress().c_str());
        AxfrDatabase::getInstance().addRdata();
    }

    return 0;
}

