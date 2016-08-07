//
// Created by marcin on 28.07.16.
//

#ifndef MYDIG_DIG_PARSER_H
#define MYDIG_DIG_PARSER_H

#include <stdio.h>
#include <stdint-gcc.h>
#include <dns/types.h>
#include <dns/message.h>

struct response
{
    char name[128];
    char ip[256];
    int32_t ttl;
    char cls[32];
    char type[32];
    char rdata[1024];
};

typedef struct response response_t;

typedef void (*parse_message_cb)(response_t* res);

int32_t parse_message(dns_message_t *msg, dns_section_t section,
                      const dns_master_style_t *style,
                      dns_messagetextflag_t flags,
                      isc_buffer_t *target,
                      parse_message_cb parse_message_fun);

#endif //MYDIG_DIG_PARSER_H
