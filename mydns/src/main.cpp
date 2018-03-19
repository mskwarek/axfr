#include <iostream>
#include <fstream>
#include <algorithm>

extern "C"
{
#include "DnsQuery/dns.h"
}

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

int getQueryTypeIdByName(const char* query_type)
{
    if(!query_type)
        return QTYPE_AXFR;
    else if(0 == strcmp(query_type, "A"))
        return T_A;
    else if(0 == strcmp(query_type, "AAAA"))
        return T_AAAA;
    else if(0 == strcmp(query_type, "NS"))
        return T_NS;
    else if(0 == strcmp(query_type, "CNAME"))
        return T_CNAME;
    else if(0 == strcmp(query_type, "SOA"))
        return T_SOA;
    else if(0 == strcmp(query_type, "PTR"))
        return T_PTR;
    else if(0 == strcmp(query_type, "MX"))
        return T_MX;
    else if(0 == strcmp(query_type, "TXT"))
        return T_TXT;
    else if(0 == strcmp(query_type, "HINFO"))
        return T_HINFO;
    else if(0 == strcmp(query_type, "RP"))
        return T_RP;
    else if(0 == strcmp(query_type, "AFSDB"))
        return T_AFSDB;
    else if(0 == strcmp(query_type, "LOC"))
        return T_LOC;
    else if(0 == strcmp(query_type, "SRV"))
        return T_SRV;
    else if(0 == strcmp(query_type, "NAPTR"))
        return T_NAPTR;
    else if(0 == strcmp(query_type, "RRSIG"))
        return T_RRSIG;
    else if(0 == strcmp(query_type, "NSEC"))
        return T_NSEC;
    else if(0 == strcmp(query_type, "DNSKEY"))
        return T_DNSKEY;
    else if(0 == strcmp(query_type, "AXFR"))
        return QTYPE_AXFR;
    else if(0 == strcmp(query_type, "ALL"))
        return QTYPE_TYPE_ALL;
    else
        return QTYPE_AXFR;
}

int getTimeout(const char* timeout)
{
    if(timeout)
        return atoi(timeout);
    else
        return 5;
}

dns_transport_type getTransportProtocolIdByName(const char* protocol_name)
{
    if(!protocol_name)
        return TRANSPORT_TYPE_TCP;
    else if(0 == strcmp(protocol_name, "UDP"))
        return TRANSPORT_TYPE_UDP;
    else
        return TRANSPORT_TYPE_TCP;
}

void printHelpPrompt()
{
    std::cout << "Scaner usage:"<<std::endl
            << "\t-o\toutput folder [mandatory]" << std::endl
            << "\t-t\ttimeout in ms [default 5s]" << std::endl
            << "\t-d\tdomain to scan [optional; scan only one domain]" << std::endl
            << "\t-n\tNS IP address [optional; needed by one-domain-scan mode]" << std::endl
            << "\t-q\tQuery type [A, AAAA, NS, CNAME, SOA, PTR, MX, TXT, HINFO, RP, AFSDB, LOC, SRV, NAPTR,\n"
            << "\t\tRRSIG, NSEC, DNSKEY, AXFR, ALL; default AXFR]" << std::endl
            << "\t-p\tTransport protocol used in scanner [default TCP]" << std::endl
            << "\t-l\tlist with records to scan in format domain_address|ns_ip [optional]" << std::endl;
}

int main(int argc, char* argv[]){
  std::string line;
  int i=0;

  static const char* output_arg = "-o";
  static const char* timeout_arg = "-t";
  static const char* domain_arg = "-d";
  static const char* ns_ip_arg = "-n";
  static const char* query_type_arg = "-q";
  static const char* transport_protocol_arg = "-p";
  static const char* input_list_arg = "-l";

  char *output_dir = getCmdOption(argv, argv + argc, output_arg);
  char *timeout = getCmdOption(argv, argv + argc, timeout_arg);
  char *domain = getCmdOption(argv, argv + argc, domain_arg);
  char *nameserver_ip = getCmdOption(argv, argv + argc, ns_ip_arg);
  char *query_type = getCmdOption(argv, argv + argc, query_type_arg);
  char *transport_protocol_name = getCmdOption(argv, argv + argc, transport_protocol_arg);
  char *path_to_input_list = getCmdOption(argv, argv + argc, input_list_arg);

  if(cmdOptionExists(argv, argv + argc, "-h")
        || !cmdOptionExists(argv, argv + argc, output_arg)
        || (!cmdOptionExists(argv, argv + argc, input_list_arg)
                && (!cmdOptionExists(argv, argv + argc, domain_arg) && !cmdOptionExists(argv, argv + argc, ns_ip_arg))))
  {
      printHelpPrompt();
      return 0;
  }

  if(cmdOptionExists(argv, argv + argc, domain_arg) && cmdOptionExists(argv, argv + argc, ns_ip_arg))
  {
      if(strlen(domain) > 0 && strlen(nameserver_ip) > 0)
      {
    // std::cout<<"Main args: "<<argv[1]<<" " <<argv[2]<<std::endl;
        ngethostbyname(domain, nameserver_ip, output_dir,
            getQueryTypeIdByName(query_type), getTimeout(timeout),
            getTransportProtocolIdByName(transport_protocol_name));
    }
  }
  else
  {
      std::string filename = path_to_input_list;
      std::ifstream inputFile(filename.c_str());
      while(std::getline(inputFile, line)){
          std::cout<<i++<<std::endl;
          std::vector<std::string> x = split(line, '|');
          ngethostbyname(x[0].c_str(), x[1].c_str(), output_dir,
                    getQueryTypeIdByName(query_type), getTimeout(timeout),
                    getTransportProtocolIdByName(transport_protocol_name));
      }
      inputFile.close();
  }
  return 0;
}
