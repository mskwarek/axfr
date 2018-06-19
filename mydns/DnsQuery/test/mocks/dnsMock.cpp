//
// Created by Marcin Skwarek on 10.02.2018.
//

#include "dnsMock.h"
//
//IMPLEMENT_FUNCTION_MOCK6(DnsFunctionMock, ngethostbyname,
//                         dns_result(const char* , const char*, const char, int, int, dns_transport_type));


IMPLEMENT_FUNCTION_MOCK6(DnsFunctionMock,readAnswers, void (dns_transport_type, unsigned char *, struct RES_RECORD *, unsigned char *, FILE *, int));