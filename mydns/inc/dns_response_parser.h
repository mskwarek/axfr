#ifndef DNS_RESPONSE_PARSER_H
#define DNS_RESPONSE_PARSER_H

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>

typedef enum
{
	DNS_RESULT_ERR,
	DNS_RESULT_NO_MEMORY,
	DNS_RESULT_OK
} dns_result;

enum
  {
    T_A=1, //Ipv4 address
    T_NS=2, //Nameserver
    T_CNAME= 5, // canonical name
    T_SOA=6, /* start of authority zone */
    T_PTR=12, /* domain name pointer */
    T_HINFO = 13,
    T_MX=15,
    T_TXT = 16,
    T_RP = 17,
    T_AFSDB = 18,
    T_AAAA  = 28,
    T_LOC = 29,
    T_SRV = 33,
    T_NAPTR = 35,
    T_RRSIG = 46,
    T_NSEC = 47,
    T_DNSKEY = 48
  };

dns_result ReadName(unsigned char* reader, size_t data_len, unsigned short type, unsigned char* dns, FILE* f);

#endif
