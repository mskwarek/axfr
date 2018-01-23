#ifndef DNS_UTILS_H
#define DNS_UTILS_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>

void ChangetoDnsNameFormat (unsigned char*, unsigned char*);
unsigned int readSOA(unsigned char* data, unsigned char* dns_packet_resp, unsigned char* name);
unsigned int parse_to_uint(unsigned char*);
unsigned int readString(unsigned char* data, unsigned short, unsigned char* dns_packet_resp, unsigned char* name);


#ifdef __cplusplus
}
#endif
#endif
