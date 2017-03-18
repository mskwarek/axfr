#include<stdio.h> //printf
#include<string.h>    //strlen
#include<stdlib.h>    //malloc
#include<sys/socket.h>    //you know what this is for
#include<arpa/inet.h> //inet_addr , inet_ntoa , ntohs etc
#include<netinet/in.h>
#include<unistd.h>    //getpid
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include<unistd.h>    //usleep
#include<fcntl.h> //fcntl


#include "dns.h"
//List of DNS Servers registered on the system
char dns_servers[10][100];
int dns_server_count = 0;
//Types of DNS resource records :)

static unsigned short dns_id = 0;//0x1122;/**< DNS query id  */
static unsigned char*  dns_buf;
static unsigned char* get_domain_name;
static unsigned long get_domain_ip;/**< Resolved ip address */
static QUERYDATA query_data;/**< Query type */

//extern NETCONF NetConf;

enum
  {
    T_A=1, //Ipv4 address
    T_NS=2, //Nameserver
    T_CNAME= 5, // canonical name
    T_SOA=6, /* start of authority zone */
    T_PTR=12, /* domain name pointer */
    T_HINFO = 13,
    T_MX=15,
    T_TXT = 16,
    T_RP = 17,
    T_AFSDB = 18,
    T_AAAA  = 28,
    T_LOC = 29,
    T_SRV = 33,
    T_NAPTR = 25,
    T_RRSIG = 46,
    T_NSEC = 47,
    T_DNSKEY = 48
    
  };//Mail server


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
  //    unsigned char adbit :1;
  
    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
typedef struct DNS_HEADER DNS_H;

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


static int dns_parse_reponse(void);/* analyze a response from DNS sever */
static unsigned char * dns_parse_question(unsigned char * cp);/* analyze a question part in resources recode */
static unsigned char * dns_answer(unsigned char *cp);/* analyze a answer part in resources recode */
static int parse_name(char* cp,char* qname, unsigned int qname_maxlen);/* analyze a qname in each part. */

//Function Prototypes
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);
void ReadName(unsigned char* reader,unsigned char* buffer,int* count, unsigned char* dst_buf, size_t data_len, unsigned short type, unsigned char* dns);
int hostname_to_ip(const char *hostname , char *ip);
void parse_ip(unsigned char* data);
void parse_ptr(unsigned char* data, unsigned short data_len, unsigned char* dns);
void parse_hinfo(unsigned char* data, unsigned short data_len);
void parse_rrsig(unsigned char* data, unsigned short data_len);
void readString(unsigned char*);
void getName(unsigned char* data, unsigned char* dns_packet_resp);

/*
 * Perform a DNS query by sending a packet
 * */
void ngethostbyname(const char *que , const char *server, int query_type)
{
    int s;
    unsigned char name[256] = {0};
    unsigned char buf[65536] = {0};
    struct hostent *he = NULL;
    struct sockaddr_in dest = {0};
 
    DNS_H *dns = NULL;
 
    s = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    printf("Get host by name\n");
    char server_ip[100] = {0};
    if( hostname_to_ip(server, server_ip) != 0){
      return;
    }
    printf("Nameserver IP: %s\n", server_ip);
    dest.sin_addr.s_addr = inet_addr(server_ip);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);

    if (connect(s,(struct sockaddr *) &dest, sizeof(dest)) < 0)
    {
        perror("ERROR connecting");
    }

    unsigned char *qname;
    struct QUESTION *qinfo = NULL;
    unsigned char host[128] = {0}; 
      //Set the DNS structure to standard queries

    snprintf(host, 128, "%s", que);
    printf("\nResolving %s\n" , host);

    dns = (struct DNS_HEADER *)&buf;

    dns_id = getpid();
    dns->id = (unsigned short) htons(dns_id);
    dns->qr = 0; //This is a query
    dns->opcode = 0; //This is a standard query
    dns->aa = 0; //Not Authoritative
    dns->tc = 0; //This message is not truncated
    dns->rd = 0; //Recursion Desired
    dns->ra = 0; //Recursion not available! hey we dont have it (lol)
    dns->z = 0;
    dns->ad = 1;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1); //we have only 1 question
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = 0;// htons(1);

    //point to the query portion
    qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
    ChangetoDnsNameFormat(qname , host);
    qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it
    qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);
    dns->len = htons((unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)-2);
    
    int len = (unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);
    int n = write(s, (char*)buf, len);
    if (n < 0)
      perror("ERROR writing to socket");

    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
    bzero(buf, 65536);

    enum{BUFSIZE=65536};
    char replyMessage[BUFSIZE] = {0};
    ssize_t numBytesRecv = 0;
    int off = 0;
    int data_length = 0;
    do
      {
	printf("start\n");
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
	    return;
	  }
	memcpy(buf+off, replyMessage, numBytesRecv);    
	off+=numBytesRecv;
	printf("%ld\n", numBytesRecv);
	printf("stop\n");
	if(off>=data_length+2 && off != 0)
	  {
	    break;
	  }
      }
    while (numBytesRecv > 0);
    close(s);

    /*    printf("\nHere is the message:n\n");
    for (int i = 0; i < off; i++)
      {
	printf("%x", buf[i]);
      }
    */
    dns_buf = buf;

    dns = (struct DNS_HEADER*) &buf;
    printf("\n n: %d, offset: %d, datalen: %d", n, off, ntohs(dns->len));


    int i , j , stop;
    unsigned char *reader;
    struct sockaddr_in a = {0};

    struct RES_RECORD *answers = NULL;
    struct RES_RECORD *auth = NULL;
    struct RES_RECORD *addit = NULL; //the replies from the DNS server

    
    printf("\nThe response contains : ");
    printf("\n %d Questions.",ntohs(dns->q_count));
    printf("\n %d Answers.",ntohs(dns->ans_count));
    printf("\n %d Authoritative Servers.",ntohs(dns->auth_count));
    printf("\n %d Additional records.\n\n",ntohs(dns->add_count));

    answers = (struct RES_RECORD*)calloc(ntohs(dns->ans_count), sizeof(struct RES_RECORD));
    auth = (struct RES_RECORD*)calloc(ntohs(dns->auth_count), sizeof(struct RES_RECORD));
    addit = (struct RES_RECORD*)calloc(ntohs(dns->add_count), sizeof(struct RES_RECORD));

    //move ahead of the dns header and the query field
    reader = &buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)];
    
    //Start reading answers
    stop=0;

    for(i=0;i<ntohs(dns->ans_count);i++)
      {
	/*
	printf("name: %02x %02x\n", *(reader), *(reader+1));
	printf("type: %02x %02x\n", *(reader+2), *(reader+3));
	printf("class: %02x %02x\n", *(reader+4), *(reader+5));
	printf("ttl: %02x %02x %02x %02x\n", *(reader+6), *(reader+7), *(reader+8), *(reader+9));
	printf("len: %02x %02x\n", *(reader+10), *(reader+11));
	*/

	getName(reader, buf+2);
	unsigned short class = ((*(reader+4) << 8) &0xFF00) | (*(reader+5) & 0xFF);
	unsigned short type = ((*(reader+2) << 8) &0xFF00) | (*(reader+3) & 0xFF);
	unsigned short name_size = ((*(reader+10) << 8) &0xFF00) | (*(reader+11) & 0xFF);
	answers[i].resource=(struct R_DATA*)(reader);
	char name[1024] = {0};
	
	reader+=12;
	
	printf("name_s: %d, ttl: %d, class: %d, type: %d\n", name_size, ntohs(answers[i].resource->ttl), class, type);
	answers[i].rdata = (unsigned char*)malloc(name_size);
	ReadName(reader,buf,&stop,answers[i].rdata, name_size, type, buf + 2);
	reader+=name_size;
      }

    free(answers);
    return;
}

/*
 * 
 * */

void ReadName(unsigned char* reader,unsigned char* buffer,int* count, unsigned char* name, size_t data_len, unsigned short type, unsigned char* dns)
{
  switch(type)
    {      
    case T_A:
      parse_ip(reader);
      break;
    case T_NS:
      break;
    case T_CNAME:
      break;
    case T_SOA:
      parse_ptr(reader, (unsigned short)data_len, dns);
      break;
    case T_PTR:
      break;
    case T_HINFO:
      parse_hinfo(reader, (unsigned short)data_len);
      break;
    case T_MX:
    case T_TXT:
    case T_RP:
    case T_AFSDB:
    case T_AAAA:
    case T_LOC:
    case T_SRV:
    case T_NAPTR:
    case T_RRSIG:
      parse_rrsig(reader, (unsigned short) data_len);
      break;
    case T_NSEC:
    case T_DNSKEY:
      break;
    }
}

void parse_ip(unsigned char* data)
{
  printf(" %d.%d.%d.%d \n\n", (int)*data, (int)*(data+1), (int)*(data+2), (int)*(data+3));
}

void parse_ptr(unsigned char* data, unsigned short data_len, unsigned char* dns)
{
  char qname[1024] = {0};
  int len = 0;

  printf("PTR: ");
  getName(data, dns);// These types all consist of a single domain name 
}

void parse_hinfo(unsigned char* data, unsigned short data_len)
{
  int len_cpu = (int)*data;
  int len_os = (int)*(data + len_cpu + 1);

  printf("cpu len: %d os len: %d", len_cpu, len_os);
}

void parse_rrsig(unsigned char* data, unsigned short data_len)
{

}
 
void getName(unsigned char* data, unsigned char* dns_packet_resp)
{
  printf("data: %02x %02x\n", *data, *(data+1));
  //printf
  
  if((uint8_t)(*data) & (uint8_t)0x0c == (uint8_t)0x0c)
    {
      unsigned short name_offset = (((*(data) << 8) &0xFF00) | (*(data+1) & 0xFF)) - 49152;
      printf("from pointer: %02x %02x offset: %d\n", *data, *(data+1), name_offset);
      data++;
      readString(dns_packet_resp + name_offset);
    }
  else
    {
      readString(data);
    }
}

void readString(unsigned char* data)
{
  unsigned char name[1024] = {0};
  unsigned int i = 0;
  printf("name data: %02x %02x\n", *data, *(data+1));
  
  while(*data != 0x00)
    {
      
      name[i++]=*data++;
    }
  name[i] = '\0';
  printf("%s\n", name);
}

/*
 * This will convert www.google.com to 3www6google3com 
 * got it :)
 * */
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) 
{
    int lock = 0 , i;
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
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h;
  int rv;

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


static int dns_parse_reponse(void)
{
  u_int i;
  DHDR dhdr;
  char* cur_ptr = dns_buf+2;


  dhdr.id = ntohs(*((u_short*)cur_ptr));
  if(dhdr.id != dns_id)
    {
      
      printf("Responsed ID != Query ID : %d ! = %d\n",dhdr.id, dns_id);

      return 0;
    }
  dns_id++;
  cur_ptr += 2;
  dhdr.flag0 = *cur_ptr++;
  dhdr.flag1 = *cur_ptr++;
  if(!(dhdr.flag0 & 0x80 && dhdr.flag0 & 0x84) || !(dhdr.flag1 & 0x80) )
    {
      
      printf("No reponse message, flag0 = 0x%02X, flag1 = 0x%02X\n",dhdr.flag0,dhdr.flag1);

      return 0;
    }
  if(dhdr.flag1 & 0x0F)
    {
      #ifdef DEBUG_DNS
      printf("Error of reponse : \n");
      switch(dhdr.flag1 & 0x0F)
	{
	case RC_FORMAT_ERROR:
	  printf("Format Error\n");
	  break;
	case RC_SERVER_FAIL:
	  printf("Server failure\n");
	  break;
	case RC_NAME_ERROR:
	  printf("Name Error\n");
	  break;
	case RC_NOT_IMPL:
	  printf("Not Implemented\n");
	  break;
	case RC_REFUSED:
	  printf("Refused\n");
	  break;
	}
#endif
      return 0;
    }

  dhdr.qdcount = ntohs(*((u_short*)cur_ptr));
  cur_ptr += 2;
  dhdr.ancount = ntohs(*((u_short*)cur_ptr));
  cur_ptr += 2;
  dhdr.nscount = ntohs(*((u_short*)cur_ptr));
  cur_ptr += 2;
  dhdr.arcount = ntohs(*((u_short*)cur_ptr));
  cur_ptr += 2;


  printf("Response : question count = %d, answer count = %d\n",dhdr.qdcount,dhdr.ancount);
  printf("Response : authority count = %d, additiional count = %d\n",dhdr.nscount,dhdr.arcount);

  /* Now parse the variable length sections */
  for(i = 0; i < dhdr.qdcount; i++)
    {
      cur_ptr = dns_parse_question(cur_ptr);// Question section
      if(!cur_ptr)
	{

	  printf("Fail to parse question section%d\n",i);

	  return 0;
	}
    }

  /* parse resource records */

  for(i=0; i < dhdr.ancount; i++)
    {
      cur_ptr = dns_answer(cur_ptr);// Answer section
      if(!cur_ptr)
	{

	  printf("Fail to parse answer section%d\n",i);

	  return 0;
	}
    }
  
  return 1;
}


/** 
    @briefParse question section in the DNS query
    @returnsuccess - 1, fail - 0 
*/
static u_char * dns_parse_question(
				   u_char * cp/**< curruent pointer to be parsed */
				   )
{
  int len;
  char name[MAX_QNAME_LEN];

  len = parse_name(cp, name, sizeof(name));
  if(!len)
    {
      printf("Fail to parse (Q)NAME field\n");
      return 0;
    }

  cp += len;
  cp += 2;// skip type
  cp += 2;// skip class
  
  printf("In question section, (Q)NAME field value : %s\n",name);

  return cp;
}


static unsigned char * dns_answer(
			   unsigned char *cp
			   )
{
  int len, type;
  char qname[MAX_QNAME_LEN];
  unsigned long tip;

  len = parse_name(cp, qname, sizeof(qname));

  if(!len) return 0;

  cp += len;
  type = *cp++;
  type = (type << 8) + (unsigned int)*cp++;// type
  cp += 2;// skip class
  cp += 4;// skip ttl
  cp += 2;// skip len

  switch(type)
    {
    case TYPE_A:
      tip = 0;
      *((unsigned char*)&tip) = *cp++;// Network odering
      *(((unsigned char*)&tip) + 1) = *cp++;
      *(((unsigned char*)&tip) + 2) = *cp++;
      *(((unsigned char*)&tip) + 3) = *cp++;
      struct in_addr ip_addr;
      ip_addr.s_addr = tip;
     
      printf("RRs : TYPE_A = %s\n", inet_ntoa(ip_addr));

      if(query_data == BYNAME) get_domain_ip = tip;
      break;
    case TYPE_CNAME:
    case TYPE_MB:
    case TYPE_MG:
    case TYPE_MR:
    case TYPE_NS:
    case TYPE_PTR:
      len = parse_name(cp, qname, sizeof(qname));// These types all consist of a single domain name
      if(!len) return 0;// convert it to ascii format
      cp += len;
      if(query_data == BYIP && type == TYPE_PTR)
	{
	  strcpy(get_domain_name,qname);
	  printf("RRs : TYPE_PTR  = %s\n",qname);
	}
      break;
    case TYPE_HINFO:
      len = *cp++;
      cp += len;

      len = *cp++;
      cp += len;
      break;
    case TYPE_MX:
      cp += 2;
      len = parse_name(cp, qname, sizeof(qname));// Get domain name of exchanger
      if(!len)
	{
	
	  printf("TYPE_MX : Fail to get domain name of exechanger\n");
	  return 0;
	}
      cp += len;
      break;
    case TYPE_SOA:
      len = parse_name(cp, qname, sizeof(qname));// Get domain name of name server
      if(!len)
	{
	  
	  printf("TYPE_SOA : Fail to get domain name of name server\n");
	  return 0;
	}

      cp += len;

      len = parse_name(cp, qname, sizeof(qname));// Get domain name of responsible person
      if(!len)
	{
	 
	  printf("TYPE_SOA : Fail to get domain name of responsible person\n");
	  return 0;
	}
      cp += len;

      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      break;
    case TYPE_TXT:
      break;// Just stash
    default:
      break;// Ignore
    }
  return cp;
}


/** 
    @briefParse answer section in the DNS query. Store resolved IP address into destination address
    @returnend address of answer section, fail - 0
*/
static int parse_name(
		      char* cp,/**< Convert a compressed domain name to the human-readable form */
		      char* qname, /**< store human-readable form(input,output); */
		      unsigned int qname_maxlen/**< qname_max_len - max length of qname(input) */
		      )
{
  unsigned int slen;// Length of current segment
  int clen = 0;// Total length of compressed name
  int indirect = 0;// Set if indirection encountered
  int nseg = 0;// Total number of label in name

  for(;;)
    {
      slen = *cp++;// Length of this segment
      if (!indirect) clen++;

      if ((slen & 0xc0) == 0xc0)// Is used in compression scheme?
	{
	  cp = &dns_buf[((slen & 0x3f)<<8) + *cp];// Follow indirection
	  if(!indirect)clen++;
	  indirect = 1;
	  slen = *cp++;
	}

      if (slen == 0)// zero length == all done
	break;

      if (!indirect) clen += slen;

      if((qname_maxlen -= slen+1) < 0)
	{
	  printf("Not enough memory\n");
	  return 0;
	}
      while (slen-- != 0) *qname++ = (char)*cp++;
      *qname++ = '.';

      nseg++;
    }

  if(nseg == 0)*qname++ = '.';// Root name; represent as single dot
  else --qname;

  *qname = '\0';
  //printf("Result of parsing (Q)NAME field : %s\n", qname);

  return clen;// Length of compressed message// Length of compressed message
}
