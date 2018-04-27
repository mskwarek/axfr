//
// Created by Marcin Skwarek on 03.03.2018.
//

#include "../inc/dns_received_packet_reader.h"
#include "../inc/dns_response_parser.h"
#include <stdio.h>
#include "utils.h"

void readAnswers(dns_transport_type transport_type, unsigned char *reader, struct RES_RECORD *answers,
                unsigned char *buf, FILE *f, int answers_cnt)
{
    //Start reading answers

    int i = 0;
    int noMemory = 0;
    int mallocRetry = 0;
    for(i=0; i<answers_cnt; )
    {
        unsigned char na[1024] = {0};
        unsigned int tcp_offset = 0;
        if(TRANSPORT_TYPE_TCP == transport_type)
        {
            tcp_offset = 2;
        }
        unsigned int name_offset = readSOA(reader, buf+tcp_offset, na) + 1;

//    	unsigned short class = ((*(reader+2 + name_offset) << 8) &0xFF00) | (*(reader+3 + name_offset) & 0xFF);
        unsigned short type = ((*(reader+name_offset) << 8) &0xFF00) | (*(reader+1 +name_offset) & 0xFF);
        unsigned int ttl = parse_to_uint(reader+4+name_offset);
        unsigned short name_size = ((*(reader+8+name_offset) << 8) &0xFF00) | (*(reader+9+name_offset) & 0xFF);
        answers[i].resource=(struct R_DATA*)(reader);

        reader+=10 + name_offset;

        answers[i].rdata = (unsigned char*)malloc(name_size);
        if(answers[i].rdata == NULL && mallocRetry < 4)
        {
            mallocRetry++;
        }
        else
        {
            fprintf(f, "%s\t%d\t%d\t", na, ttl, type);
        }
        if((DNS_RESULT_NO_MEMORY != ReadName(reader, name_size, type, buf + tcp_offset, f)) || noMemory > 2)
        {
            reader+=name_size;
            noMemory = 0;
            mallocRetry = 0;
            ++i;
        }
        else
        {
            noMemory++;
        }
    }
}
