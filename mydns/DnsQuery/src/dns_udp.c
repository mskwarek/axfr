//
// Created by Marcin Skwarek on 11.03.2018.
//

#include <string.h>
#include <sys/socket.h> //you know what this is for
#include <arpa/inet.h>
#include <unistd.h>

#include "../inc/dns_udp.h"
#include "utils.h"

dns_result dns_udp_req(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    unsigned int to, char *host, char *buf, int query_type, const char *server)
{
    int s = -1;
    struct sockaddr_in dest = {0};

    feel_dns_header_req(&dns->header);

    // point to the query portion
    qname = (unsigned char *)&buf[sizeof(struct DNS_UDP_HEADER)];
    ChangetoDnsNameFormat(qname, (unsigned char *)host);
    qinfo = (struct QUESTION
            *)&buf[sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1)]; // fill it
    qinfo->qtype = htons(query_type); // type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);

    int i = 0;
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP packet for DNS queries
    if (s < 0)
    {
        // printf("Conn refused\n");
        close(s);
        return DNS_RESULT_CONN_REFUSED;
    }

    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(server);

    struct timeval tv = {0};
    tv.tv_sec = to;
    int setopterr = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
    if (setopterr < 0)
    {
        // perror("Error");
        // printf("err: %d\n", setopterr);
        // printf("setsockopt failed");
        return DNS_RESULT_SOCKET_ERR;
    }

    int len = (unsigned int)sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1) +
              sizeof(struct QUESTION);

    if (0 > sendto(s, (char *)buf, len, 0, (struct sockaddr *)&dest, sizeof(dest)))
    {
        // printf("sendto failed");
        close(s);
        return DNS_RESULT_SEND_ERR;
    }

    // Receive the answer
    i = sizeof dest;
    if (recvfrom(s, (char *)buf, 65536, 0, (struct sockaddr *)&dest, (socklen_t *)&i) < 0)
    {
        // perror("recvfrom failed");
        close(s);
        // printf("recvfrom failed");
        return DNS_RESULT_RECV_ERR;
    }
    close(s);
    return DNS_RESULT_OK;
}
