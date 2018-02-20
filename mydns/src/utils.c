#include "utils.h"
#include<string.h>

static void convert_name(unsigned char *name);

/*
 * This will convert www.google.com to 3www6google3com
 * got it :)
 * */
void ChangetoDnsNameFormat(unsigned char* dns, unsigned char* host)
{
    if(NULL == dns || NULL == host)
        return;

    int lock = 0 , i = 0;

    int passed_host_addr_len = strlen((char*) host);

    if('.' != host[passed_host_addr_len-1])
    {
        strcat((char*)host, ".");
        passed_host_addr_len++;
    }


    for(i = 0 ; i < passed_host_addr_len ; i++)
    {
        if(host[i]=='.')
        {
            if(i == 0)
                return;

            *dns++ = i-lock;
            for(;lock<i;lock++)
            {
                *dns++=host[lock];
            }
            lock++; //or lock=i+1;
        }
    }
    *dns++='\0';
}


unsigned int readSOA(unsigned char* data, unsigned char* dns_packet_resp, unsigned char* name)
{
    unsigned int p = 0, all = 0;
    //printf("name data: %02x %02x  ", *data, *(data+1));

    while(*data != 0x00)
    {
        if((uint8_t)(*data) >= 192)
        {
            unsigned short name_offset = (((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF)) - 49152;
            //printf("from pointer: %02x %02x offset: %d ", *data, *(data+1), name_offset);
            all+=readString(dns_packet_resp + name_offset, 0, dns_packet_resp, name+p);
            p+=1;
            break;
        }
        ++all;
        name[p++]=*data++;
    }
    name[all] = '\0';

    //now convert 3www6google3com0 to www.google.com
    convert_name(name);
//    name[i-1]='\0'; //remove the last dot

    return p;
}

unsigned int parse_to_uint(unsigned char* data)
{
  return (unsigned int)*data << 24 |(unsigned int)*(data+1) << 16 |
    (unsigned int)*(data+2) << 8 | (unsigned int)*(data+3);
}

void convert_name(unsigned char *name)
{
    unsigned int i = 0, j = 0, all = 0;
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        all=name[i];
        for(j=0;j<(int)all;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    if(i>0)
    {
        name[i-1]='\0';
    }
}

unsigned int readString(unsigned char* data, unsigned short data_len, unsigned char* dns_packet_resp, unsigned char* name)
{
    unsigned int p = 0;
    //printf("name data: %02x %02x  ", *data, *(data+1));

    while(*data != 0x00)
    {
        if((uint8_t)(*data) >= 192 && *(data+1) != 0x00)
        {
            unsigned short name_offset = (((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF)) - 49152;
            //printf("from pointer: %02x %02x offset: %d ", *data, *(data+1), name_offset);
            p+=readString(dns_packet_resp + name_offset, 0, dns_packet_resp, name+p);
            break;
        }

        name[p++]=*data++;
    }

    //printf("while success\n");
    name[p] = '\0';
    return p;
}
