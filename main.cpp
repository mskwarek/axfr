//
// Created by marcin on 27.07.16.
//

#include "Scanner/axfrLookup.h"
#include "Scanner/AxfrDatabase.hpp"
#include "Scanner/InputReader.h"

int main()
{
    axfrLookup* lookup = new axfrLookup();
    InputReader* reader = new InputReader();
    reader->ReadFromFile("/home/marcin/mgr/test");

    std::cout<<"main"<<std::endl;
    for(auto i : reader->GetData())
    {
        lookup->performLookup(i.GetDomainAddress().c_str(), i.GetNsAddress().c_str());
    }

    delete lookup;
    delete reader;

    return 0;
}

