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

//List of DNS Servers registered on the system
char dns_servers[10][100];
int dns_server_count = 0;
//Types of DNS resource records :)
 
enum
  {
    T_A=1, //Ipv4 address
    T_NS=2, //Nameserver
    T_CNAME= 5, // canonical name
    T_SOA=6, /* start of authority zone */
    T_PTR=12, /* domain name pointer */
    T_MX=15
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

typedef struct
{
  unsigned char *name;
  unsigned short type;
  unsigned short payload_size;
  unsigned char *rcode;
  unsigned char *edns0ver;
  unsigned short z;
  unsigned short datalen;
} ADDITIONAL_Q;

//Function Prototypes
void ChangetoDnsNameFormat (unsigned char*,unsigned char*);
u_char* ReadName(unsigned char* reader,unsigned char* buffer,int* count);
int hostname_to_ip(const char *hostname , char *ip);
void get_dns_servers();
/*
 * Perform a DNS query by sending a packet
 * */
void ngethostbyname(const char *que , const char *server, int query_type)
{
  printf("Arguments: %s, %s\n", que, server);

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
    printf("Nameserver IP: %s", server_ip);
    dest.sin_addr.s_addr = inet_addr(server_ip);
    dest.sin_family = AF_INET;
    dest.sin_port = htons(53);
    /*
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 5000;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      perror("Error");
    }
    //dest.sin_addr.s_addr = inet_addr(server); //dns servers
    */
    if (connect(s,(struct sockaddr *) &dest, sizeof(dest)) < 0)
    {
        perror("ERROR connecting");
    }

    unsigned char *qname;
    struct QUESTION *qinfo = NULL;
    unsigned char host[128] = {0}; 
      //Set the DNS structure to standard queries

    snprintf(host, 128, "%s", que);

    printf("Resolving %s\n" , host);

    //buf = (unsigned char*)calloc(65536);
    dns = (struct DNS_HEADER *)&buf;
 
    dns->id = (unsigned short) htons(getpid());
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
    printf("Build ended\n");
    //point to the query portion
    qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
    printf("qname e\n");
    ChangetoDnsNameFormat(qname , host);
    printf("format changed\n");
    qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)]; //fill it
    printf("qinfo build\n");
    qinfo->qtype = htons( query_type ); //type of the query , A , MX , CNAME , NS etc
    qinfo->qclass = htons(1);
    printf("query type set\n");
    dns->len = htons((unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)-2);
    
    int len = (unsigned int)sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);
    int n = write(s, (char*)buf, len);
    if (n < 0)
      perror("ERROR writing to socket");

    
    int timeout = 10;
    int byteRead = 0;
    bzero(buf, 65536);

    int size_recv , total_size= 0;
    struct timeval begin , now;
    enum { CHUNK_SIZE =65536 };
    char chunk[CHUNK_SIZE] = {0};
    double timediff;
    int offset = 0; 

    enum{BUFSIZE=65536};
    char replyMessage[BUFSIZE];
    ssize_t numBytesRecv = 0;
    int off = 0;
    do
      {
	numBytesRecv = recv(s, replyMessage, BUFSIZE, 0);
	if ( numBytesRecv < 0)
	  printf("recv() failed\n");
	memcpy(buf+off, replyMessage, numBytesRecv);    
	off+=numBytesRecv;
	memset(&replyMessage, 0, sizeof(replyMessage));
      }
    while (numBytesRecv > 0);
   
    dns = (struct DNS_HEADER*) &buf;

    printf("\nHere is the message:n\n");
    for (int i = 0; i < off; i++)
      {
	printf("%x", buf[i]);
      }
    
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
    reader = &buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)-2];

    //Start reading answers
    stop=0;

    for(i=0;i<ntohs(dns->ans_count);i++)
      {
	answers[i].name=ReadName(reader,buf,&stop);
	reader = reader + stop;

	answers[i].resource = (struct R_DATA*)(reader);
	reader = reader + sizeof(struct R_DATA);

	if(ntohs(answers[i].resource->type) == 1) //if its an ipv4 address
	  {
	    answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->data_len));

	    for(j=0 ; j<ntohs(answers[i].resource->data_len) ; j++)
	      {
		answers[i].rdata[j]=reader[j];
	      }

	    answers[i].rdata[ntohs(answers[i].resource->data_len)] = '\0';

	    reader = reader + ntohs(answers[i].resource->data_len);
	  }
	else
	  {
	    answers[i].rdata = ReadName(reader,buf,&stop);
	    reader = reader + stop;
	  }
      }

    //read authorities
    for(i=0;i<ntohs(dns->auth_count);i++)
      {
	auth[i].name=ReadName(reader,buf,&stop);
	reader+=stop;

	auth[i].resource=(struct R_DATA*)(reader);
	reader+=sizeof(struct R_DATA);

	auth[i].rdata=ReadName(reader,buf,&stop);
	reader+=stop;
      }

    //read additional
    for(i=0;i<ntohs(dns->add_count);i++)
      {
	addit[i].name=ReadName(reader,buf,&stop);
	reader+=stop;

	addit[i].resource=(struct R_DATA*)(reader);
	reader+=sizeof(struct R_DATA);

	if(ntohs(addit[i].resource->type)==1)
	  {
	    addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->data_len));
	    for(j=0;j<ntohs(addit[i].resource->data_len);j++)
	      addit[i].rdata[j]=reader[j];

	    addit[i].rdata[ntohs(addit[i].resource->data_len)]='\0';
	    reader+=ntohs(addit[i].resource->data_len);
	  }
	else
	  {
	    addit[i].rdata=ReadName(reader,buf,&stop);
	    reader+=stop;
	  }
      }

    //print answers
    printf("\nAnswer Records : %d \n" , ntohs(dns->ans_count) );
    for(i=0 ; i < ntohs(dns->ans_count) ; i++)
      {
	printf("Name : %s ",answers[i].name);

	if( ntohs(answers[i].resource->type) == T_A) //IPv4 address
	  {
	    long *p;
	    p=(long*)answers[i].rdata;
	    a.sin_addr.s_addr=(*p); //working without ntohl
	    printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
	  }

	if(ntohs(answers[i].resource->type)==5)
	  {
	    //Canonical name for an alias
	    printf("has alias name : %s",answers[i].rdata);
	  }

	printf("\n");
      }

    //print authorities
    printf("\nAuthoritive Records : %d \n" , ntohs(dns->auth_count) );
    for( i=0 ; i < ntohs(dns->auth_count) ; i++)
      {

	printf("Name : %s ",auth[i].name);
	if(ntohs(auth[i].resource->type)==2)
	  {
	    printf("has nameserver : %s",auth[i].rdata);
	  }
	printf("\n");
      }

    //print additional resource records
    printf("\nAdditional Records : %d \n" , ntohs(dns->add_count) );
    for(i=0; i < ntohs(dns->add_count) ; i++)
      {
	printf("Name : %s ",addit[i].name);
	if(ntohs(addit[i].resource->type)==1)
	  {
	    long *p;
	    p=(long*)addit[i].rdata;
	    a.sin_addr.s_addr=(*p);
	    printf("has IPv4 address : %s",inet_ntoa(a.sin_addr));
	  }
	printf("\n");
      }

    //{
    free(answers);
	//}
	//for(i=0;i<ntohs(dns->add_count);++i)
	//{
    free(addit);
    //  free(buf);
    return;
}

/*
 * 
 * */

u_char* ReadName(unsigned char* reader,unsigned char* buffer,int* count)
{
  unsigned char *name;
  unsigned int p=0,jumped=0,offset;
  int i , j;

  *count = 1;
  name = (unsigned char*)malloc(256);

  name[0]='\0';

  //read the names in 3www6google3com format
  while(*reader!=0)
    {
      if(*reader>=192)
	{
	  offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 ;)
	  reader = buffer + offset - 1;
	  jumped = 1; //we have jumped to another location so counting wont go up!
	}
      else
	{
	  name[p++]=*reader;
	}

      reader = reader+1;

      if(jumped==0)
	{
	  *count = *count + 1; //if we havent jumped to another location then we can count up
	}
    }

  name[p]='\0'; //string complete
  if(jumped==1)
    {
      *count = *count + 1; //number of steps we actually moved forward in the packet
    }

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
  name[i-1]='\0'; //remove the last dot
  return name;
}
/*
 * Get the DNS servers from /etc/resolv.conf file on Linux
 * */
void get_dns_servers()
{
    FILE *fp;
    char line[200] , *p;
    if((fp = fopen("/etc/resolv.conf" , "r")) == NULL)
    {
        printf("Failed opening /etc/resolv.conf file \n");
    }
     
    while(fgets(line , 200 , fp))
    {
        if(line[0] == '#')
        {
            continue;
        }
        if(strncmp(line , "nameserver" , 10) == 0)
        {
            p = strtok(line , " ");
            p = strtok(NULL , " ");
             
            //p now is the dns ip :)
            //????
        }
    }
     
    strcpy(dns_servers[0] , "208.67.222.222");
    strcpy(dns_servers[1] , "208.67.220.220");
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
      printf("IP: %s", ip);
    }

  freeaddrinfo(servinfo); // all done with this structure
  return 0;
}
