//
// Created by marcin on 27.07.16.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "axfrLookup.h"


extern "C"
{
    #include "digQuery.h"
};

void int_parse(response_t* res)
{
    std::vector<std::string> tokens;

    std::string sentence(res->rdata);
    std::istringstream iss(sentence);
    std::copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         back_inserter(tokens));

    for (auto const &n : tokens)
    {
        if(!(res->cls == "SOA"))
            std::cout << ' ' << n << ' ' << res->cls << ' ' << res->type;
    }
    std::cout<<std::endl;
}

void axfrLookup::performLookup()
{
    tryLookup(int_parse);
}