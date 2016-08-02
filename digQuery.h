//
// Created by marcin on 27.07.16.
//

#ifndef MYDIG_DIGQUERY_H
#define MYDIG_DIGQUERY_H

#include <dig_parser.h>

void tryLookup(const char* domain, const char* asked_ns, parse_message_cb parse_message_fun);
response_t getResponse();

#endif //MYDIG_DIGQUERY_H
