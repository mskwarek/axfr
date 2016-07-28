//
// Created by marcin on 28.07.16.
//

#ifndef MYDIG_DIG_PARSER_H
#define MYDIG_DIG_PARSER_H

int32_t parse_message(dns_message_t *msg, dns_section_t section,
                      const dns_master_style_t *style,
                      dns_messagetextflag_t flags,
                      isc_buffer_t *target);

#endif //MYDIG_DIG_PARSER_H
