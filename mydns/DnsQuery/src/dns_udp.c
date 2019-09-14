//
// Created by Marcin Skwarek on 11.03.2018.
//

#include <string.h>
#include <sys/socket.h> //you know what this is for
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <netinet/ip6.h>
#include <sys/ioctl.h>

#include "../inc/dns_udp.h"
#include "utils.h"

unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
}

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

    // printf("\nSending Packet...");
    if (0 > sendto(s, (char *)buf, len, 0, (struct sockaddr *)&dest, sizeof(dest)))
    {
        printf("sendto failed");
        close(s);
        return DNS_RESULT_ERR;
    }
    // printf("Done");

    // Receive the answer
    i = sizeof dest;
    // printf("\nReceiving answer...");
    if (recvfrom(s, (char *)buf, 65536, 0, (struct sockaddr *)&dest, (socklen_t *)&i) < 0)
    {
        // perror("recvfrom failed");
        close(s);
        printf("recvfrom failed");
        return DNS_RESULT_ERR;
    }
    // printf("Done");
    close(s);
    return DNS_RESULT_OK;
}

uint16_t checksum(uint16_t *addr, int len)
{
    int count = len;
    register uint32_t sum = 0;
    uint16_t answer = 0;

    // Sum up 2-byte values until none or only one byte left.
    while (count > 1)
    {
        sum += *(addr++);
        count -= 2;
    }

    // Add left-over byte, if any.
    if (count > 0)
    {
        sum += *(uint8_t *)addr;
    }

    // Fold 32-bit sum into 16 bits; we lose information by doing this,
    // increasing the chances of a collision.
    // sum = (lower 16 bits) + (upper 16 bits shifted right 16 bits)
    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    // Checksum is one's compliment of sum.
    answer = ~sum;

    return (answer);
}

// Build IPv6 UDP pseudo-header and call checksum function (Section 8.1 of RFC 2460).
uint16_t udp6_checksum(struct ip6_hdr iphdr, struct udphdr udphdr, uint8_t *payload, int payloadlen)
{
    char buf[IP_MAXPACKET];
    char *ptr;
    int chksumlen = 0;
    int i;

    ptr = &buf[0]; // ptr points to beginning of buffer buf

    // Copy source IP address into buf (128 bits)
    memcpy(ptr, &iphdr.ip6_src.s6_addr, sizeof(iphdr.ip6_src.s6_addr));
    ptr += sizeof(iphdr.ip6_src.s6_addr);
    chksumlen += sizeof(iphdr.ip6_src.s6_addr);

    // Copy destination IP address into buf (128 bits)
    memcpy(ptr, &iphdr.ip6_dst.s6_addr, sizeof(iphdr.ip6_dst.s6_addr));
    ptr += sizeof(iphdr.ip6_dst.s6_addr);
    chksumlen += sizeof(iphdr.ip6_dst.s6_addr);

    // Copy UDP length into buf (32 bits)
    memcpy(ptr, &udphdr.len, sizeof(udphdr.len));
    ptr += sizeof(udphdr.len);
    chksumlen += sizeof(udphdr.len);

    // Copy zero field to buf (24 bits)
    *ptr = 0;
    ptr++;
    *ptr = 0;
    ptr++;
    *ptr = 0;
    ptr++;
    chksumlen += 3;

    // Copy next header field to buf (8 bits)
    memcpy(ptr, &iphdr.ip6_nxt, sizeof(iphdr.ip6_nxt));
    ptr += sizeof(iphdr.ip6_nxt);
    chksumlen += sizeof(iphdr.ip6_nxt);

    // Copy UDP source port to buf (16 bits)
    memcpy(ptr, &udphdr.source, sizeof(udphdr.source));
    ptr += sizeof(udphdr.source);
    chksumlen += sizeof(udphdr.source);

    // Copy UDP destination port to buf (16 bits)
    memcpy(ptr, &udphdr.dest, sizeof(udphdr.dest));
    ptr += sizeof(udphdr.dest);
    chksumlen += sizeof(udphdr.dest);

    // Copy UDP length again to buf (16 bits)
    memcpy(ptr, &udphdr.len, sizeof(udphdr.len));
    ptr += sizeof(udphdr.len);
    chksumlen += sizeof(udphdr.len);

    // Copy UDP checksum to buf (16 bits)
    // Zero, since we don't know it yet
    *ptr = 0;
    ptr++;
    *ptr = 0;
    ptr++;
    chksumlen += 2;

    // Copy payload to buf
    memcpy(ptr, payload, payloadlen * sizeof(uint8_t));
    ptr += payloadlen;
    chksumlen += payloadlen;

    // Pad to the next 16-bit boundary
    for (i = 0; i < payloadlen % 2; i++, ptr++)
    {
        *ptr = 0;
        ptr++;
        chksumlen++;
    }

    return checksum((uint16_t *)buf, chksumlen);
}

dns_result dns_req_with_spoofed_ip(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    char *host, char *buf, int query_type, const char *server, const char *spoofed_ip)
{
    printf("Start spoofing\n");
    int s = -1;
    struct sockaddr_in dest = {0};
    char datagram[sizeof(struct ip) + sizeof(struct udphdr) + sizeof(struct DNS_UDP_HEADER) +
                  65536] = {0};

    feel_dns_header_req(&dns->header);

    // point to the query portion
    qname = (unsigned char *)&buf[sizeof(struct DNS_UDP_HEADER)];
    ChangetoDnsNameFormat(qname, (unsigned char *)host);
    qinfo = (struct QUESTION
            *)&buf[sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1)]; // fill it
    qinfo->qtype = htons(query_type); // type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);

    int i = 0;
    s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW); // UDP packet for DNS queries SOCK_RAW, IPPROTO_RAW
    if (s < 0)
    {
        printf("Conn refused\n");
        close(s);
        return DNS_RESULT_ERR;
    }

    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(server);

    int len = (unsigned int)sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1) +
              sizeof(struct QUESTION);

    struct ip *ip_hdr = (struct ip *)datagram;
    struct udphdr *udp_hdr = (struct udphdr *)(datagram + sizeof(struct ip));

    ip_hdr->ip_hl = 5;                                                // header length
    ip_hdr->ip_v = 4;                                                 // version
    ip_hdr->ip_tos = 0;                                               // tos
    ip_hdr->ip_len = sizeof(struct ip) + sizeof(struct udphdr) + len; // length
    ip_hdr->ip_id = 0;                                                // id
    ip_hdr->ip_off = 0;                                               // fragment offset
    ip_hdr->ip_ttl = 255;                                             // ttl
    ip_hdr->ip_p = 17;                                                // protocol
    ip_hdr->ip_sum = 0;                                               // temp checksum
    ip_hdr->ip_src.s_addr = inet_addr(spoofed_ip);                    // src ip - spoofed
    ip_hdr->ip_dst.s_addr = inet_addr(server);                        // dst ip

    srand(time(NULL)); // Initialization, should only be called once.
    int r = rand() % 64511;
    udp_hdr->source = htons(r + 1025);
    udp_hdr->dest = htons(53);                         // srand(time(NULL)) % 65536; //dst port
    udp_hdr->len = htons(sizeof(struct udphdr) + len); // length
    udp_hdr->check = 0;                                // checksum - disabled

    ip_hdr->ip_sum = csum((unsigned short *)datagram, ip_hdr->ip_len >> 1); // real checksum
    memcpy(datagram + sizeof(struct ip) + sizeof(struct udphdr), buf, len);
    len += sizeof(struct ip);
    len += sizeof(struct udphdr);
    int on = 1;
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
    {
        perror("[-] Error! Cannot set IP_HDRINCL");
        return DNS_RESULT_ERR;
    }

    /* send spoofed packet (set routing flags to 0) */
    if (sendto(s, datagram, len, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0)
    {
        printf("sendto failed");
        close(s);
        return DNS_RESULT_SEND_ERR;
    }
    // else
    //     printf( "[+] Spoofed IP packet sent successfully!\n");

    close(s);
}

dns_result dns_req_with_spoofed_ipv6(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo,
    char *host, char *buf, int query_type, const char *server, const char *spoofed_ip)
{
    // printf("Start spoofing v6\n");

    int sock;
    int on = 1;
    int sd;
    int bytes = 0;
    socklen_t clilen;
    struct sockaddr_in6 server_addr, client_addr;
    char buffer[1024];
    char addrbuf[INET6_ADDRSTRLEN];
    char datagram[sizeof(struct ip6_hdr) + sizeof(struct udphdr) + sizeof(struct DNS_UDP_HEADER) +
                  65536] = {0};
    struct ifreq ifr;
    unsigned char interface[128] = {0};

    // Interface to send packet through.
    strcpy(interface, "ens3");

    // Submit request for a socket descriptor to look up interface.
    if ((sd = socket(AF_INET6, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        // perror("socket() failed to get socket descriptor for using ioctl() ");
        // exit(EXIT_FAILURE);
        return DNS_RESULT_ERR;
    }
    // Use ioctl() to look up interface name and get its MAC address.
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", interface);
    if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)
    {
        // perror("ioctl() failed to get source MAC address ");
        // return (EXIT_FAILURE);
        return DNS_RESULT_ERR;
    }
    close(sd);
    int8_t src_mac[6] = {0};
    // Copy source MAC address.
    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6 * sizeof(uint8_t));

    // Report source MAC address to stdout.
    printf("MAC address for interface %s is ", interface);
    for (int i = 0; i < 5; i++)
    {
        printf("%02x:", src_mac[i]);
    }
    printf("%02x\n", src_mac[5]);

    struct sockaddr_ll device;
    // Find interface index from interface name and store index in
    // struct sockaddr_ll device, which will be used as an argument of sendto().
    memset(&device, 0, sizeof(device));
    if ((device.sll_ifindex = if_nametoindex(interface)) == 0)
    {
        // perror("if_nametoindex() failed to obtain interface index ");
        // exit(EXIT_FAILURE);
        return DNS_RESULT_ERR;
    }
    printf("Index for interface %s is %i\n", interface, device.sll_ifindex);
    uint8_t dst_mac[6] = {0};
    // Set destination MAC address: you need to fill this out
    dst_mac[0] = 0x84;
    dst_mac[1] = 0xc1;
    dst_mac[2] = 0xc1;
    dst_mac[3] = 0x1e;
    dst_mac[4] = 0x67;
    dst_mac[5] = 0xf0;

    device.sll_family = AF_PACKET;
    device.sll_protocol = htons(ETH_P_IPV6);
    memcpy(device.sll_addr, dst_mac, 6 * sizeof(uint8_t));
    device.sll_halen = 6;

    /* create a DGRAM (UDP) socket in the INET6 (IPv6) protocol */
    sock = socket(PF_INET6, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0)
    {
        // perror("creating socket");
        return DNS_RESULT_ERR;
    }

    memset(&server_addr, 0, sizeof(server_addr));

    if (1 != inet_pton(AF_INET6, server, &(server_addr.sin6_addr)))
        printf("Inet_pton failed");

    feel_dns_header_req(&dns->header);

    // point to the query portion
    qname = (unsigned char *)&buf[sizeof(struct DNS_UDP_HEADER)];
    ChangetoDnsNameFormat(qname, (unsigned char *)host);
    qinfo = (struct QUESTION
            *)&buf[sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1)]; // fill it
    qinfo->qtype = htons(query_type); // type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);

    int len = (unsigned int)sizeof(struct DNS_UDP_HEADER) + (strlen((const char *)qname) + 1) +
              sizeof(struct QUESTION);

    struct ip6_hdr *ip_hdr = (struct ip6_hdr *)datagram;
    struct udphdr *udp_hdr = (struct udphdr *)(datagram + sizeof(struct ip6_hdr));

    ip_hdr->ip6_flow = htonl((6 << 28) | (0 << 20) | 512709);
    ip_hdr->ip6_plen = htons(sizeof(struct udphdr) + len); // length
    ip_hdr->ip6_nxt = 17;                                  // protocol
    ip_hdr->ip6_hops = 255;

    int status = 0;
    struct in6_addr src = {0};
    if ((status = inet_pton(AF_INET6, spoofed_ip, &(src))) != 1)
    {
        fprintf(stderr, "inet_pton() failed.\nError message: %s", strerror(status));
        // exit(EXIT_FAILURE);
        return DNS_RESULT_ERR;
    }
    memcpy(&ip_hdr->ip6_src, &src, 16);

    // Destination IPv6 address (128 bits)
    if ((status = inet_pton(AF_INET6, server, &(ip_hdr->ip6_dst))) != 1)
    {
        fprintf(stderr, "inet_pton() failed.\nError message: %s", strerror(status));
        // exit(EXIT_FAILURE);
        return DNS_RESULT_ERR;
    }

    srand(time(NULL)); // Initialization, should only be called once.
    int r = rand() % 64511;
    udp_hdr->source = htons(r + 1025);
    udp_hdr->dest = htons(53);                         // srand(time(NULL)) % 65536; //dst port
    udp_hdr->len = htons(sizeof(struct udphdr) + len); // length
    udp_hdr->check = udp_hdr->check = udp6_checksum(*ip_hdr, *udp_hdr, buf, len);

    const int IP6_HDRLEN = 40;
    const int UDP_HDRLEN = 8;
    int frame_length = IP6_HDRLEN + UDP_HDRLEN + len;
    unsigned char ether_frame[85536] = {0};

    // IPv6 header
    memcpy(ether_frame, ip_hdr, IP6_HDRLEN * sizeof(uint8_t));

    // UDP header
    memcpy(ether_frame + IP6_HDRLEN, udp_hdr, UDP_HDRLEN * sizeof(uint8_t));

    // UDP data
    memcpy(ether_frame + IP6_HDRLEN + UDP_HDRLEN, buf, len * sizeof(uint8_t));

    // Open raw socket descriptor.
    if ((sd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL))) < 0)
    {
        perror("socket() failed ");
        exit(EXIT_FAILURE);
    }

    // Send ethernet frame to socket.
    if ((bytes = sendto(
             sd, ether_frame, frame_length, 0, (struct sockaddr *)&device, sizeof(device))) <= 0)
    {
        perror("sendto() failed");
        exit(EXIT_FAILURE);
    }

    close(sd);
}
