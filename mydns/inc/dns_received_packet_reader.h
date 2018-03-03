//
// Created by Marcin Skwarek on 03.03.2018.
//

#ifndef AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H
#define AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H

#include "dns.h"

void readAnswers(dns_transport_type transport_type, unsigned char *reader, struct RES_RECORD *answers,
                 unsigned char *buf, FILE *f, int answers_cnt);

#endif //AXFRSCANNER_DNS_RECEIVED_PACKET_READER_H
