//
// Created by Marcin Skwarek on 10.02.2018.
//

#pragma once

#include "cmock/cmock.h"
#include <stdio.h>
#include "../inc/dns_received_packet_reader.h"

// DECLARE_FUNCTION_MOCK6(DnsFunctionMock, ngethostbyname, dns_result(const char* , const char*,
//    const char, int, int, dns_transport_type));
//

DECLARE_FUNCTION_MOCK6(DnsFunctionMock, readAnswers,
    void(dns_transport_type, unsigned char *, struct RES_RECORD *, unsigned char *, FILE *, int));