//
// Created by Marcin Skwarek on 08.03.2018.
//

#include "SocketMock.h"

CMOCK_MOCK_FUNCTION6(SocketMock, sendto, ssize_t(int, const void *, size_t, int, const struct sockaddr *, socklen_t));
CMOCK_MOCK_FUNCTION6(SocketMock, recvfrom, ssize_t(int, void *, size_t, int, struct sockaddr *__restrict, socklen_t *__restrict));
CMOCK_MOCK_FUNCTION8(SocketMock, dns_tcp_req,
dns_result(DNS_H_TCP *, unsigned char *, struct QUESTION *, unsigned int, char *,
    unsigned char *, int, const char *));
CMOCK_MOCK_FUNCTION3(SocketMock, socket, int(int, int, int));
CMOCK_MOCK_FUNCTION5(SocketMock, setsockopt, int(int, int, int, const void *, socklen_t));
CMOCK_MOCK_FUNCTION3(SocketMock, connect, int(int, const struct sockaddr *, socklen_t));

