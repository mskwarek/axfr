//
// Created by Marcin Skwarek on 10.02.2018.
//

#include "dnsMock.h"

CMOCK_MOCK_FUNCTION7(DnsFunctionMock, readAnswers, void(dns_transport_type, unsigned char *, struct RES_RECORD *, unsigned char *, char *, size_t, int))
