//
// Created by Marcin Skwarek on 11.03.2018.
//

#include "dns_packet.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void feel_dns_header_req(dns_header* dns)
{
    if(NULL == dns)
    {
        return;
    }

    unsigned short dns_id = getpid();

    dns->id = (unsigned short) htons(dns_id);
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 1; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;// htons(1);
}
