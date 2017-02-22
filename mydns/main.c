#include<dns.h>

int main( int argc , char *argv[])
{
    unsigned char name[1000] = {0};
    snprintf(name, 1000, "%s", "onet.pl");
     
    //Now get the ip of this hostname , A record
    ngethostbyname(name, "8.8.8.8", 1);
 
    return 0;
}
