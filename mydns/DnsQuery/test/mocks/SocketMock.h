//
// Created by Marcin Skwarek on 08.03.2018.
//

#ifndef AXFRSCANNER_SOCKETMOCK_H
#define AXFRSCANNER_FILEMOCK_H

#include <cmock/cmock.h>
#include "sys/socket.h"
#include "stdio.h"
#include "dns_tcp.h"
#include "proxy_functions.h"


class SocketMock : public CMockMocker<SocketMock>
{
public:
    MOCK_METHOD6(sendto, ssize_t(int, const void *, size_t, int, const struct sockaddr *, socklen_t));
    MOCK_METHOD6(recvfrom, ssize_t(int, void *, size_t, int, struct sockaddr *__restrict, socklen_t *__restrict));
    MOCK_METHOD8(dns_tcp_req,
    dns_result(DNS_H_TCP *, unsigned char *, struct QUESTION *, unsigned int, char *,
        unsigned char *, int, const char *));
    MOCK_METHOD3(socket, int(int, int, int));
    MOCK_METHOD5(setsockopt, int(int, int, int, const void *, socklen_t));
    MOCK_METHOD3(connect, int(int, const struct sockaddr *, socklen_t));
};

#endif
