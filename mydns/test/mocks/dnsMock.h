//
// Created by Marcin Skwarek on 10.02.2018.
//

#ifndef AXFRSCANNER_DNSMOCK_H
#define AXFRSCANNER_DNSMOCK_H

#include "cmock/cmock.h"
#include "dns.h"

DECLARE_FUNCTION_MOCK6(DnsFunctionMock, ngethostbyname, dns_result(const char* , const char*, const char, int, int, dns_transport_type));

#endif //AXFRSCANNER_DNSMOCK_H
