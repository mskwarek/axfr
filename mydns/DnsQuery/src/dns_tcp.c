//
// Created by Marcin Skwarek on 11.03.2018.
//

#include <string.h>
#include <arpa/inet.h>
#include "../inc/dns_tcp.h"
#include "utils.h"
#include <unistd.h>
#include <fcntl.h> //fcntl
#include <errno.h>
#include "../../SystemFunctionProxy/inc/proxy_functions.h"

dns_result dns_tcp_req(DNS_H_TCP *dns, unsigned char *qname, struct QUESTION *qinfo,
    unsigned int to, char *host, unsigned char *buf, int query_type, const char *server)
{
    int s = -1;
    struct sockaddr_in dest = {0};

    enum
    {
        BUFSIZE = 65536,
        DEF_DNS_PORT = 53
    };

    // point to the query portion
    ChangetoDnsNameFormat(qname, (unsigned char *)host);
    qinfo = (struct QUESTION
            *)&buf[sizeof(struct DNS_TCP_HEADER) + (strlen((const char *)qname) + 1)]; // fill it
    qinfo->qtype = htons(query_type); // type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);
    dns->len = htons((unsigned int)sizeof(struct DNS_TCP_HEADER) +
                     (strlen((const char *)qname) + 1) + sizeof(struct QUESTION) - 2);

    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s < 0)
    {
        printf("Conn refused\n");
        return DNS_RESULT_ERR;
    }
    struct timeval timeout;
    timeout.tv_sec = to;
    timeout.tv_usec = 0;

    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        printf("setsockopt failed\n");
        return DNS_RESULT_ERR;
    }

    // printf("Nameserver IP: %s\n", server);
    dest.sin_addr.s_addr = inet_addr(server);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(DEF_DNS_PORT);

    long arg = 0;
    int res = 0;
    fd_set myset = {{0}};
    int valopt = 0;

    socklen_t lon = {0};

    if ((arg = int_fcntl(s, F_GETFL, NULL)) < 0)
    {
        return DNS_RESULT_ERR;
        // exit(0);
    }
    arg |= O_NONBLOCK;
    if (int_fcntl(s, F_SETFL, arg) < 0)
    {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        return DNS_RESULT_ERR;
    }
    // Trying to connect with timeout
    res = connect(s, (struct sockaddr *)&dest, sizeof(dest));
    if (res < 0)
    {
        if (errno == EINPROGRESS)
        {
            // fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
            do
            {
                timeout.tv_sec = to;
                timeout.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(s, &myset);
                res = select(s + 1, NULL, &myset, NULL, &timeout);
                if (res < 0 && errno != EINTR)
                {
                    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                    close(s);
                    return DNS_RESULT_ERR;
                }
                else if (res > 0)
                {
                    // Socket selected for write
                    lon = sizeof(int);
                    if (getsockopt(s, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &lon) < 0)
                    {
                        fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                        close(s);
                        return DNS_RESULT_ERR;
                    }
                    // Check the value returned...
                    if (valopt)
                    {
                        fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt,
                            strerror(valopt));
                        close(s);
                        return DNS_RESULT_ERR;
                    }
                    break;
                }
                else
                {
                    fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    close(s);
                    return DNS_RESULT_ERR;
                }
            } while (1);
        }
        else
        {
            fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
            close(s);
            return DNS_RESULT_ERR;
        }
    }
    // Set to blocking mode again...
    if ((arg = fcntl(s, F_GETFL, NULL)) < 0)
    {
        fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        close(s);
        return DNS_RESULT_ERR;
    }
    arg &= (~O_NONBLOCK);
    if (fcntl(s, F_SETFL, arg) < 0)
    {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        close(s);
        return DNS_RESULT_ERR;
    }

    if (connect(s, (struct sockaddr *)&dest, sizeof(dest)) < 0)
    {
        printf("ERROR connecting\n");
        close(s);
        return DNS_RESULT_ERR;
    }

    int len = (unsigned int)sizeof(struct DNS_TCP_HEADER) + (strlen((const char *)qname) + 1) +
              sizeof(struct QUESTION);
    int n = write(s, (char *)buf, len);
    if (n < 0)
    {
        perror("ERROR writing to socket");
    }
    bzero(buf, BUFSIZE);

    char replyMessage[BUFSIZE] = {0};
    ssize_t numBytesRecv = 0;
    int off = 0;
    int data_length = 0;
    do
    {
        memset(&replyMessage, 0, sizeof(replyMessage));
        numBytesRecv = recv(s, replyMessage, BUFSIZE, 0);
        if (off == 0 && numBytesRecv >= 2)
        {
            data_length = ((*(replyMessage) << 8) & 0xFF00) | (*(replyMessage + 1) & 0xFF);
        }
        if (numBytesRecv < 0)
        {
            printf("recv() failed\n");
            close(s);
            return DNS_RESULT_ERR;
        }
        memcpy(buf + off, replyMessage, numBytesRecv);
        off += numBytesRecv;
        // printf("%ld\n", numBytesRecv);
        if (off >= data_length + 2 && off != 0)
        {
            break;
        }
    } while (numBytesRecv > 0);
    close(s);

    return DNS_RESULT_OK;
}
