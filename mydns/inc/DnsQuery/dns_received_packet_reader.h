//
// Created by Marcin Skwarek on 03.03.2018.
//

#ifndef AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H
#define AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dns.h"

void readAnswers(dns_transport_type transport_type, unsigned char *reader, struct RES_RECORD *answers,
                 unsigned char *buf, FILE *f, int answers_cnt);

#ifdef __cplusplus
}
#endif

#endif //AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H
