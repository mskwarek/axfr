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
    const char* dupa = "ns3.infusionsoft.com";
    for(auto i : reader.GetData())
    {
        dupa = i.GetNsAddress().c_str();
        std::cout<<i.GetDomainAddress() << ", " << i.GetNsAddress()<<std::endl;
        lookup.performLookup(i.GetDomainAddress().c_str(), i.GetNsAddress().c_str());
        AxfrDatabase::getInstance().addRdata();
    }

    return 0;
}

