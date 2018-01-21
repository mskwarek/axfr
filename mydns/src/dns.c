#include<stdio.h> //printf
#include<string.h>    //strlen
#include<stdlib.h>
#include<sys/socket.h>    //you know what this is for
#include<arpa/inet.h> //inet_addr , inet_ntoa , ntohs etc
#include<netinet/in.h>
#include<unistd.h>    //getpid
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include<unistd.h>    //usleep
#include<fcntl.h> //fcntl
#include <errno.h>
#include <sys/time.h>
#include "dns.h"

//List of DNS Servers registered on the system
char dns_servers[10][100];
int dns_server_count = 0;
//Types of DNS resource records :)

static unsigned short dns_id = 0;//0x1122;/**< DNS query id  */
static unsigned char*  dns_buf;

//DNS header structure
struct DNS_HEADER
{
    unsigned short len;
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
typedef struct DNS_HEADER DNS_H;

struct DNS_HEADER_UDP
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
typedef struct DNS_HEADER_UDP DNS_H_UDP;

struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};

//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)

//Pointers to resource record contents
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};

//Structure of a Query
typedef struct
{
    unsigned char *name;
    struct QUESTION *ques;
} QUERY;

//Function Prototypes
void convert_name(unsigned char *name);
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);
dns_result ReadName(unsigned char* reader, size_t data_len, unsigned short type, unsigned char* dns, FILE* f);
int hostname_to_ip(const char *hostname , char *ip);
void parse_ip(unsigned char* data, FILE* f);
void parse_ptr(unsigned char* data, unsigned short data_len, unsigned char* dns, FILE* f);
void parse_soa(unsigned char* data, unsigned short data_len, unsigned char* dns, FILE* f);
void parse_hinfo(unsigned char* data, unsigned short data_len, FILE* f);
void parse_rrsig(unsigned char* data, unsigned char*, unsigned short data_len, FILE* f);
unsigned int readString(unsigned char* data, unsigned short, unsigned char* dns_packet_resp, unsigned char* name);
int getName(unsigned char* data, unsigned short data_len, unsigned char* dns_packet_resp, FILE* f);
void parse_ns(unsigned char* data, unsigned short, unsigned char* dns, FILE* f);
void parse_mx(unsigned char* data, unsigned short data_len, unsigned char* dns_packet_resp, FILE* f);
dns_result parse_txt(unsigned char* data, unsigned short data_len, FILE* f);
unsigned int readSOA(unsigned char* data, unsigned char* dns_packet_resp, unsigned char* name);
void parse_nsec(unsigned char* data, unsigned char*, FILE* f);
void parse_cname(unsigned char* data, unsigned char* dns, FILE* f);
void parse_rp(unsigned char* data, unsigned char*, FILE* f);
void parse_dnskey(unsigned char*, unsigned short, FILE* f);
void parse_afsdb(unsigned char* data, unsigned char*, FILE* f);
void parse_aaaa(unsigned char* data, FILE* f);
void parse_loc(unsigned char* data, FILE* f);
void parse_srv(unsigned char* data, unsigned char*, FILE* f);
dns_result parse_naptr(unsigned char* data, unsigned short, FILE* f);
unsigned int parse_to_uint(unsigned char*);
void parse_default(unsigned char* data, unsigned short data_len, FILE* f);


dns_result ngethostbyname(const char *que , const char *server, const char *dst_log_path, int query_type, int to, dns_transport_type transport_type)
{
    int s = -1;
    unsigned char buf[65536] = {0};
    struct sockaddr_in dest = {0};
    int answers_cnt = 0;
    int dns_header_size = 0;

    printf("%s %s\n", que, server);

    unsigned char *qname = NULL;
    struct QUESTION *qinfo = NULL;
    char host[128] = {0};
      //Set the DNS structure to standard queries

    snprintf(host, 128, "%s", que);
    // printf("\nResolving %s\n" , host);

    if(TRANSPORT_TYPE_UDP == transport_type)
    {
        DNS_H_UDP *dns = NULL;
        dns = (struct DNS_HEADER_UDP *)&buf;

        dns_id = getpid();
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

        //point to the query portion
        qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER_UDP)];
        ChangetoDnsNameFormat(qname , (unsigned char*)host);
        qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER_UDP) + (strlen((const char*)qname) + 1)]; //fill it
        qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
        qinfo->qclass = htons(1);

        int i = 0;
        s = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries
        dest.sin_family = AF_INET;
        dest.sin_port = htons(53);
        dest.sin_addr.s_addr = inet_addr(server);

        struct timeval tv;
        tv.tv_sec = to;
        int setopterr = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
        if (setopterr < 0) {
            //perror("Error");
            //printf("err: %d\n", setopterr);
            printf("setsockopt failed");
            return DNS_RESULT_ERR;
        }

        int len = (unsigned int)sizeof(struct DNS_HEADER_UDP) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);

        printf("\nSending Packet...");
        if( sendto(s,(char*)buf,len,0,(struct sockaddr*)&dest,sizeof(dest)) < 0)
        {
            printf("sendto failed");
            return DNS_RESULT_ERR;
        }
        printf("Done");

        //Receive the answer
        i = sizeof dest;
        printf("\nReceiving answer...");
        if(recvfrom (s,(char*)buf , 65536 , 0 , (struct sockaddr*)&dest , (socklen_t*)&i ) < 0)
        {
            //perror("recvfrom failed");
            printf("recvfrom failed");
            return DNS_RESULT_ERR;
        }
        printf("Done");

        dns_buf = buf;
        dns = (struct DNS_HEADER_UDP*) &buf;

        answers_cnt = ntohs(dns->ans_count);
        dns_header_size = sizeof(struct DNS_HEADER_UDP);
    }
    else
    {
        DNS_H *dns = NULL;

        dns = (struct DNS_HEADER *)&buf;
        dns_id = getpid();
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

        //point to the query portion
        qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
        ChangetoDnsNameFormat(qname , (unsigned char*)host);
        qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it
        qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
        qinfo->qclass = htons(1);
        dns->len = htons((unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)-2);


        s = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
        if(s<0)
        {
            printf("Conn refused");
            return DNS_RESULT_ERR;
        }
        struct timeval timeout;
        timeout.tv_sec = to;
        timeout.tv_usec = 0;

        if (setsockopt (s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0) {
            printf("setsockopt failed\n");
        }

        // printf("Nameserver IP: %s\n", server);
        dest.sin_addr.s_addr = inet_addr(server);
        dest.sin_family = AF_INET;
        dest.sin_port = htons(53);

        long arg = 0;
        int res = 0;
        fd_set myset = {{0}};
        int valopt = 0;

        socklen_t lon = {0};

        if( (arg = fcntl(s, F_GETFL, NULL)) < 0) {
            fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
            return DNS_RESULT_ERR;
            // exit(0);
        }
        arg |= O_NONBLOCK;
        if( fcntl(s, F_SETFL, arg) < 0) {
            fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
            return DNS_RESULT_ERR;
        }
        // Trying to connect with timeout
        res = connect(s, (struct sockaddr *)&dest, sizeof(dest));
        if (res < 0) {
            if (errno == EINPROGRESS) {
                // fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
                do {
                    timeout.tv_sec = to;
                    timeout.tv_usec = 0;
                    FD_ZERO(&myset);
                    FD_SET(s, &myset);
                    res = select(s+1, NULL, &myset, NULL, &timeout);
                    if (res < 0 && errno != EINTR) {
                        fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                        close(s);
                        return DNS_RESULT_ERR;
                    }
                    else if (res > 0) {
                        // Socket selected for write
                        lon = sizeof(int);
                        if (getsockopt(s, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
                            fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                            close(s);
                            return DNS_RESULT_ERR;
                        }
                        // Check the value returned...
                        if (valopt) {
                            fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt)
                            );
                            close(s);
                            return DNS_RESULT_ERR;
                        }
                        break;
                    }
                    else {
                        fprintf(stderr, "Timeout in select() - Cancelling!\n");
                        close(s);
                        return DNS_RESULT_ERR;
                    }
                } while (1);
            }
            else {
                fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                close(s);
                return DNS_RESULT_ERR;
            }
        }
        // Set to blocking mode again...
        if( (arg = fcntl(s, F_GETFL, NULL)) < 0) {
            fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
            close(s);
            return DNS_RESULT_ERR;
        }
        arg &= (~O_NONBLOCK);
        if( fcntl(s, F_SETFL, arg) < 0) {
            fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
            close(s);
            return DNS_RESULT_ERR;
        }


        if (connect(s,(struct sockaddr *) &dest, sizeof(dest)) < 0)
        {
            printf("ERROR connecting\n");
            close(s);
            return DNS_RESULT_ERR;
        }


        int len = (unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);
        int n = write(s, (char*)buf, len);
        if (n < 0) {
            perror("ERROR writing to socket");
        }

        bzero(buf, 65536);

        enum{BUFSIZE=65536};
        char replyMessage[BUFSIZE] = {0};
        ssize_t numBytesRecv = 0;
        int off = 0;
        int data_length = 0;
        do
          {
    	memset(&replyMessage, 0, sizeof(replyMessage));
    	numBytesRecv = recv(s, replyMessage, BUFSIZE, 0);
    	if(off == 0 && numBytesRecv >= 2)
    	  {
    	    data_length = ((*(replyMessage) << 8) &0xFF00) | (*(replyMessage+1) & 0xFF);
    	  }
    	if ( numBytesRecv < 0)
    	  {
    	    printf("recv() failed\n");
    	    close(s);
    	    return DNS_RESULT_ERR;
    	  }
    	memcpy(buf+off, replyMessage, numBytesRecv);
    	off+=numBytesRecv;
    	// printf("%ld\n", numBytesRecv);
    	if(off>=data_length+2 && off != 0)
    	  {
    	    break;
    	  }
          }
        while (numBytesRecv > 0);
        close(s);
        dns_buf = buf;

        dns = (struct DNS_HEADER*) &buf;
        answers_cnt = ntohs(dns->ans_count);
        dns_header_size = sizeof(struct DNS_HEADER);
    }

    //printf("\ndatalen: %d\n", ntohs(dns->len));


    int i =0;
    unsigned char *reader = NULL;
    struct sockaddr_in a = {0};

    struct RES_RECORD *answers = NULL;

    char filename[512] = {0};
    snprintf(filename, 512, "%s/%s_%s.axfr", dst_log_path, que, server);
    //printf(" %s\n", filename);

    if(answers_cnt <= 0)
    {
        //printf("answers: %d", answers_cnt);
	    return DNS_RESULT_ERR;
    }

    if(TRANSPORT_TYPE_TCP == transport_type)
    {
        unsigned short dnsIdFromPacket = ((*(buf+2) << 8) &0xFF00) | (*(buf+3) & 0xFF);

        if(dns_id != dnsIdFromPacket)
        {
            printf("dnsId from resp does not match");
            return DNS_RESULT_ERR;
        }
    }
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return DNS_RESULT_ERR;
    }
    //
    // printf("\nThe response contains : ");
    // printf("\n %d Answers.", answers_cnt);
    //

    answers = (struct RES_RECORD*)calloc(answers_cnt, sizeof(struct RES_RECORD));
    if(answers == NULL)
    {
        fclose(f);
        return DNS_RESULT_NO_MEMORY;
    }

    //move ahead of the dns header and the query field
    reader = &buf[dns_header_size + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)];

    //Start reading answers

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

    	unsigned short class = ((*(reader+2 + name_offset) << 8) &0xFF00) | (*(reader+3 + name_offset) & 0xFF);
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
    fclose(f);
    for(i=0; i < answers_cnt; ++i)
    {
        if(answers[i].rdata!=NULL)
            free(answers[i].rdata);
    }
    if(answers!=NULL)
        free(answers);
    return DNS_RESULT_OK;
}

/*
 *
 * */

dns_result ReadName(unsigned char* reader,size_t data_len, unsigned short type, unsigned char* dns, FILE* f)
{
    if(type < 1 || type > 256)
    {
        return DNS_RESULT_ERR;
    }
  switch(type)
    {
    case T_A:
      parse_ip(reader, f);
      break;
    case T_NS:
      //printf("datalen: %d", data_len);
      parse_ns(reader, (unsigned short)data_len, dns, f);
      break;
    case T_CNAME:
      parse_cname(reader, dns, f);
      break;
    case T_SOA:
      parse_soa(reader, (unsigned short)data_len, dns, f);
      break;
    case T_PTR:
      parse_ptr(reader, (unsigned short)data_len, dns, f);
      break;
    case T_HINFO:
      parse_hinfo(reader, (unsigned short)data_len, f);
      break;
    case T_MX:
        parse_mx(reader, (unsigned short)data_len, dns, f);
        break;
    case T_TXT:
        return parse_txt(reader, data_len, f);
        break;
    case T_RP:
      parse_rp(reader, dns, f);
      break;
    case T_AFSDB:
      parse_afsdb(reader, dns, f);
      break;
    case T_AAAA:
      parse_aaaa(reader, f);
      break;
    case T_LOC:
      parse_loc(reader, f);
      break;
    case T_SRV:
      parse_srv(reader, dns, f);
      break;
    case T_NAPTR:
      return parse_naptr(reader, (unsigned short) data_len, f);
      break;
    case T_RRSIG:
      parse_rrsig(reader, dns, (unsigned short) data_len, f);
      break;
    case T_NSEC:
      parse_nsec(reader, dns, f);
      break;
    case T_DNSKEY:
      parse_dnskey(reader, (unsigned short)data_len, f);
      break;
    default:
      parse_default(reader, (unsigned short) data_len, f);
      break;
    }
    return DNS_RESULT_OK;
}

unsigned short parse_to_ushort(unsigned char* data)
{
  return ((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF);
}
void parse_default(unsigned char* data, unsigned short data_len, FILE* f)
{
  int i = 0;
  if(data_len ==0)
  {
      return;
  }

  while(i < data_len)
    {
      fprintf(f, "%02x", *(data+i));
      ++i;
    }
  fprintf(f, "\n");
}
void parse_dnskey(unsigned char* data, unsigned short data_len, FILE* f)
{
  unsigned short flags = parse_to_ushort(data);
  unsigned char protocol = *(data+2);
  unsigned char algorithm = *(data+3);
  int i = 0;
  while(i < data_len - 4)
    {
      fprintf(f, "%02x", *(data+i+4));
      ++i;
    }
  fprintf(f, "\n");
}

void parse_rp(unsigned char* data, unsigned char* dns, FILE* f)
{
  unsigned char mailbox[1024] = {0};
  unsigned char txt_rr[1024] = {0};

  int x = readSOA(data, dns, mailbox);
  readSOA(data+x+1, dns, txt_rr);
  fprintf(f, "%s %s\n", mailbox, txt_rr);
}

void parse_afsdb(unsigned char* data, unsigned char* dns, FILE* f)
{
  unsigned short subtype = parse_to_ushort(data);
  unsigned char hostname[1024] = {0};
  readSOA(data+2, dns, hostname);
  fprintf(f, "%u %s\n", subtype, hostname);;
}

void parse_aaaa(unsigned char* data, FILE* f)
{
    fprintf(f, "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n", *(data), *(data+1), *(data+2), *(data+3), *(data+4), *(data+5), *(data+6), *(data+7), *(data+8), *(data+9), *(data+10), *(data+11), *(data+12), *(data+13), *(data+14), *(data+15));
}

void parse_loc(unsigned char* data, FILE* f)
{
  uint8_t size = (uint8_t)*data+1;
  uint8_t version = (uint8_t)*data;
  uint8_t hor_precision = (uint8_t)*data+2;
  uint8_t ver_precision = (uint8_t)*data+3;
  unsigned int latitude = parse_to_uint(data+4);
  unsigned int longitude = parse_to_uint(data+8);
  unsigned int altitude = parse_to_uint(data+12);
    fprintf(f, "%u %u %u %u %u %u %u\n", version, size, hor_precision, ver_precision, latitude, longitude, altitude );
}

void parse_srv(unsigned char* data, unsigned char *dns, FILE* f)
{
  unsigned short priority = parse_to_ushort(data);
  unsigned short weight = parse_to_ushort(data+2);
  unsigned short port = parse_to_ushort(data+4);
  unsigned char name[512] = {0};
  readSOA(data+6, dns, name);
    fprintf(f, "%u %u %u %s", priority, weight, port, name);
    fprintf(f, "\n");
}

dns_result parse_naptr(unsigned char* data, unsigned short data_len, FILE* f)
{
    unsigned short order = parse_to_ushort(data);
    data+=2;
    unsigned short preference = parse_to_ushort(data);
    data+=2;
    unsigned char flags_length = *(data) & 0xFF;
    ++data;
    unsigned char *txt= NULL;
    unsigned char *txt2= NULL;

    unsigned char *txt3= NULL;
    unsigned char *txt4= NULL;

    int i =0;

    txt = (unsigned char*)calloc(flags_length+1, sizeof(unsigned char));
    if(txt==NULL)
    {
        return DNS_RESULT_NO_MEMORY;
    }
    while(i<flags_length)
    {
        txt[i++]=*data++;

    }
    txt[i]='\0';

    unsigned char service_len = *(data) & 0xFF;
    ++data;
    i=0;
    txt2 = (unsigned char*)calloc(service_len+1, sizeof(unsigned char));
    if(txt2 == NULL)
    {
        if(txt!=NULL)
            free(txt);
        return DNS_RESULT_NO_MEMORY;
    }
    while(i<service_len)
    {
        if(txt2!=NULL)
            txt2[i++]=*data++;
    }
    txt2[i]='\0';

    unsigned char regex_len = *(data) & 0xFF;
    ++data;
    i=0;
    txt3 = (unsigned char*)calloc(regex_len+1, sizeof(unsigned char));
    if(txt3 == NULL )
    {
        if(txt!=NULL)
            free(txt);
        if(txt2!=NULL)
            free(txt2);
        return DNS_RESULT_NO_MEMORY;
    }
    while(i<regex_len)
    {
        txt3[i++]=*data++;
    }
    txt3[i]='\0';

    i=0;
    int rep_len = data_len - 5 - 1 -1 - 1 -service_len-regex_len;
    // ++data;

    txt4 = (unsigned char*)calloc(rep_len+1, sizeof(unsigned char));
    if(txt4 == NULL)
    {
        if(txt!=NULL)
            free(txt);
        if(txt2!=NULL)
            free(txt2);
        if(txt3!=NULL)
            free(txt3);
        return DNS_RESULT_NO_MEMORY;
    }
    while(i<rep_len)
    {
      txt4[i++]=*data++;
    }
    txt4[i]='\0';

    //convert_name(txt4);

    fprintf(f,"%u %u %d %d %s %s %s %s\n", order, preference, flags_length, service_len, txt, txt2, txt3, txt4);


    if(txt!=NULL)
        free(txt);
    if(txt2!=NULL)
        free(txt2);
    if(txt3!=NULL)
        free(txt3);
    if(txt4!=NULL)
        free(txt4);

    return DNS_RESULT_OK;
}

void parse_ip(unsigned char* data, FILE* f)
{
    fprintf(f,"%d.%d.%d.%d\n", (int)*data, (int)*(data+1), (int)*(data+2), (int)*(data+3));
}

void parse_ns(unsigned char* data, unsigned short data_len, unsigned char* dns, FILE* f)
{
  getName(data, data_len, dns, f);
    fprintf(f,"\n");
}

void parse_ptr(unsigned char* data, unsigned short data_len, unsigned char* dns, FILE* f)
{
  getName(data, data_len, dns, f);// These types all consist of a single domain name
    fprintf(f,"\n");
}

void parse_cname(unsigned char* data, unsigned char* dns, FILE* f)
{
  unsigned char name[512] = {0};
  readSOA(data, dns, name);
    fprintf(f,"%s\n", name);
}


unsigned int parse_to_uint(unsigned char* data)
{
  return (unsigned int)*data << 24 |(unsigned int)*(data+1) << 16 |
    (unsigned int)*(data+2) << 8 | (unsigned int)*(data+3);
}

void parse_soa(unsigned char* data, unsigned short data_len, unsigned char* dns, FILE* f)
{
    unsigned char name[1024] = {0};
    unsigned int p = 0;

    p = readSOA(data, dns, name);
    fprintf(f," %s ", name);
    memset(name, 0, 1024);
    p = readSOA(data+p+1, dns, name);
    fprintf(f,"%s ", name);

    unsigned int serial_no = parse_to_uint(data+p+3);
    unsigned int refresh = parse_to_uint(data+p+7);
    unsigned int retry = parse_to_uint(data+p+11);
    unsigned int expire = parse_to_uint(data+p+15);
    unsigned int min_ttl = parse_to_uint(data+p+19);
    fprintf(f,"%u %u %u %u %u", serial_no, refresh, retry, expire, min_ttl);
    fprintf(f,"\n");
}

void parse_hinfo(unsigned char* data, unsigned short data_len, FILE* f)
{
  int len_cpu = (int)*data;
  int len_os = (int)*(data + len_cpu + 1);

    fprintf(f,"cpu len: %d os len: %d ", len_cpu, len_os);
      unsigned char algorithm = *(data+3);
  int i = 0;
  while(i < data_len)
    {
      fprintf(f, "%02x", *(data+i));
      ++i;
    }
    fprintf(f,"\n");
}

void parse_mx(unsigned char* data, unsigned short data_len, unsigned char* dns_packet_resp, FILE* f)
{
    unsigned short preference = ((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF);
    getName(data+2, data_len, dns_packet_resp, f);
    fprintf(f,"\n");
}

dns_result parse_txt(unsigned char* data, unsigned short data_len, FILE* f)
{
    unsigned int i = 0;
    unsigned char *txt= NULL;

    txt = (unsigned char*)calloc(data_len, sizeof(unsigned char));
    if(txt == NULL)
    {
        return DNS_RESULT_NO_MEMORY;
    }
    data++;
    while(i<data_len-1)
    {
        txt[i++]=*data++;
    }

    fprintf(f,"%s", txt);
    fprintf(f,"\n");
    if(txt!=NULL)
        free(txt);
    return DNS_RESULT_OK;
}

void parse_rrsig(unsigned char* data, unsigned char* dns, unsigned short data_len, FILE* f)
{

  //TODO CONVERT TIME
    unsigned short type = ((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF);
    unsigned char algorithm = *(data + 2)& 0xFF;
    unsigned char labels = *(data + 3) & 0xFF;
    unsigned int ttl = parse_to_uint(data+4);
    unsigned int timestamp_exp = parse_to_uint(data+8);
    unsigned int timestamp_inc = parse_to_uint(data+12);
    unsigned short key_tag = type = ((*(data + 16) << 8) &0xFF00) | (*(data+17) & 0xFF);
    unsigned char name[1024] = {0};
    unsigned int p = 0;

    p = readSOA(data+18, dns, name);

    fprintf(f,"%u %u %u %u %u %u %u %s ", type, algorithm, labels, ttl, timestamp_exp, timestamp_inc, key_tag, name);
    int i = 0;
    while(i < data_len - p - 19)
    {
        fprintf(f,"%02x", *(data+p+i+19));
  	++i;
    }
    fprintf(f,"\n");
}

void parse_nsec(unsigned char* data, unsigned char* dns, FILE* f)
{
  unsigned char name[512] = {0};
  readSOA(data, dns, name);
    fprintf(f,"%s\n", name);
}

int getName(unsigned char* data, unsigned short data_len, unsigned char* dns_packet_resp, FILE* f)
{
    unsigned char name[1024] = {0};
    unsigned int i = 0, j = 0, p = 0;

    p = readString(data, data_len, dns_packet_resp, name);
    //printf("after readString");

    //now convert 3www6google3com0 to www.google.com
    for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    if(i > 0)
    {
        name[i-1]='\0'; //remove the last dot
    }

    fprintf(f,"%s", name);
    return strlen((char*)name);
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


/*
 * This will convert www.google.com to 3www6google3com
 * got it :)
 * */
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host)
{
    int lock = 0 , i = 0;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
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

int hostname_to_ip(const char *hostname , char *ip)
{
  struct addrinfo hints = {0}, *servinfo = NULL, *p = NULL;
  struct sockaddr_in *h = NULL;
  int rv = 0;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
  hints.ai_socktype = SOCK_STREAM;

  if ( (rv = getaddrinfo( hostname , NULL , NULL , &servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
    {
      h = (struct sockaddr_in *) p->ai_addr;
      strcpy(ip , inet_ntoa( h->sin_addr ) );
      if(strcmp(ip, "0.0.0.0") != 0)
	{
	  freeaddrinfo(servinfo);
	  return 0;
	}
      printf("IP: %s\n", ip);
    }

  freeaddrinfo(servinfo); // all done with this structure
  return 0;
}
