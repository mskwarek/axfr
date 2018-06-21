//
// Created by Marcin Skwarek on 08.03.2018.
//

#include "SocketMock.h"

//IMPLEMENT_FUNCTION_MOCK5(SocketFunctionMock, setsockopt, int(int, int, int, const void *, socklen_t));

IMPLEMENT_FUNCTION_MOCK6(SendtoFunctionMock, sendto, ssize_t(int, const void *, size_t, int, const struct sockaddr *,
    socklen_t));
IMPLEMENT_FUNCTION_MOCK6(RecvfromFunctionMock, recvfrom, ssize_t(int, void *, size_t, int, struct sockaddr * __restrict,
                    socklen_t * __restrict));

IMPLEMENT_FUNCTION_MOCK8(DnsTcpReceivedDataMock, dns_tcp_req, dns_result (DNS_H_TCP *, unsigned char *,
    struct QUESTION *, unsigned int, char*, unsigned char*, int, const char *));


IMPLEMENT_FUNCTION_MOCK3(SocketCreateMock, socket, int(int, int, int));