//
// Created by Marcin Skwarek on 10.02.2018.
//

#ifndef AXFRSCANNER_DNSFUNCTIONMOCK_H
#define AXFRSCANNER_DNSFUNCTIONMOCK_H

#include <cmock/cmock.h>
#include <stdio.h>
#include "dns.h"

class DnsFunctionMock : public CMockMocker<DnsFunctionMock>
{
public:
    MOCK_METHOD7(readAnswers, void(dns_transport_type, unsigned char *, struct RES_RECORD *, unsigned char *, char *, size_t, int));
};

#endif
