//
// Created by marcin on 27.07.16.
//

#ifndef MYDIG_AXFRLOOKUP_H
#define MYDIG_AXFRLOOKUP_H

extern "C"
{
#include <dig_parser.h>
};

class axfrLookup {
public:
    void performLookup(const char* domain, const char* asked_ns);
};


#endif //MYDIG_AXFRLOOKUP_H
