#include <algorithm>
#include <fstream>
#include <iostream>

extern "C" {
#include "../DnsQuery/inc/dns.h"
}

#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <future>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "spdlog/spdlog.h"
#include <chrono>
#include <deque>

char *getCmdOption(char **begin, char **end, const std::string &option)
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

template<typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

int getQueryTypeIdByName(const char *query_type)
{
    if (!query_type)
        return QTYPE_AXFR;

    struct
    {
        const char *dns_query_name;
        uint8_t query_number;
    } query_map[] = {{"A", T_A}, {"AAAA", T_AAAA}, {"NS", T_NS}, {"CNAME", T_CNAME}, {"SOA", T_SOA},
        {"PTR", T_PTR}, {"MX", T_MX}, {"TXT", T_TXT}, {"HINFO", T_HINFO}, {"RP", T_RP},
        {"AFSDB", T_AFSDB}, {"LOC", T_LOC}, {"SRV", T_SRV}, {"NAPTR", T_NAPTR}, {"RRSIG", T_RRSIG},
        {"NSEC", T_NSEC}, {"DNSKEY", T_DNSKEY}, {"AXFR", QTYPE_AXFR}, {"ALL", QTYPE_TYPE_ALL}};

    for (uint32_t i = 0; i < sizeof(query_map) / sizeof(query_map[0]); ++i)
    {
        spdlog::debug(std::string(query_type) + " " + std::string(query_map[i].dns_query_name));
        if (0 == strcmp(query_type, query_map[i].dns_query_name))
            return query_map[i].query_number;
    }
    return QTYPE_AXFR;
}

int getTimeout(const char *timeout)
{
    if (timeout)
        return atoi(timeout);
    else
        return 5;
}

int getWorkers(const char *workers)
{
    if (workers)
        return atoi(workers);
    else
        return 250;
}

dns_transport_type getTransportProtocolIdByName(const char *protocol_name)
{
    if (!protocol_name)
        return TRANSPORT_TYPE_TCP;

    if (0 == strcmp(protocol_name, "UDP"))
        return TRANSPORT_TYPE_UDP;

    return TRANSPORT_TYPE_TCP;
}

void printHelpPrompt()
{
    std::cout << "Scaner usage:" << std::endl
              << "\t-o\toutput folder [if no param responses won't be saved]" << std::endl
              << "\t-t\ttimeout in ms [default 5s]" << std::endl
              << "\t-d\tdomain to scan [optional; scan only one domain]" << std::endl
              << "\t-n\tNS IP address [optional; needed by one-domain-scan mode]" << std::endl
              << "\t-q\tQuery type [A, AAAA, NS, CNAME, SOA, PTR, MX, TXT, HINFO, "
                 "RP, AFSDB, LOC, SRV, NAPTR,\n"
              << "\t\tRRSIG, NSEC, DNSKEY, AXFR, ALL; default AXFR]" << std::endl
              << "\t-p\tTransport protocol used in scanner [default TCP]" << std::endl
              << "\t-l\tlist with records to scan in format domain_address|ns_ip "
                 "[optional]"
              << "\t-s\tlist with records to scan in format domain_address|ns_ip [optional]|spoofed_src_ip" << std::endl;
}

struct Job
{
    std::chrono::time_point<std::chrono::high_resolution_clock> schedule_time;
    std::shared_future<dns_result> job;
};

int main(int argc, char *argv[])
{
    std::string line;
    int i = 0;

    static const char *output_arg = "-o";
    static const char *timeout_arg = "-t";
    static const char *domain_arg = "-d";
    static const char *ns_ip_arg = "-n";
    static const char *query_type_arg = "-q";
    static const char *transport_protocol_arg = "-p";
    static const char *input_list_arg = "-l";
    static const char *workers_arg = "-w";
    static const char* spoof_list_arg = "-s";

    char *output_dir = getCmdOption(argv, argv + argc, output_arg);
    char *timeout = getCmdOption(argv, argv + argc, timeout_arg);
    char *domain = getCmdOption(argv, argv + argc, domain_arg);
    char *nameserver_ip = getCmdOption(argv, argv + argc, ns_ip_arg);
    char *query_type = getCmdOption(argv, argv + argc, query_type_arg);
    char *transport_protocol_name = getCmdOption(argv, argv + argc, transport_protocol_arg);
    char *path_to_input_list = getCmdOption(argv, argv + argc, input_list_arg);
    char *workers_counted = getCmdOption(argv, argv + argc, workers_arg);
    response_dump_to_file dump_to_file = RESPONSE_DUMP;
    char *path_to_input_list_with_spoofed_src = getCmdOption(argv, argv + argc, spoof_list_arg);

    if (cmdOptionExists(argv, argv + argc, output_arg) == false)
        dump_to_file = RESPONSE_DO_NOT_DUMP;

    const uint16_t workers = getWorkers(workers_counted);

    if (cmdOptionExists(argv, argv + argc, "-h") ||
        (!cmdOptionExists(argv, argv + argc, input_list_arg) &&
            (!cmdOptionExists(argv, argv + argc, domain_arg) &&
                !cmdOptionExists(argv, argv + argc, ns_ip_arg)))
            && !cmdOptionExists(argv, argv + argc, spoof_list_arg))
    {
        printHelpPrompt();
        return 0;
    }

    if (cmdOptionExists(argv, argv + argc, domain_arg) &&
        cmdOptionExists(argv, argv + argc, ns_ip_arg))
    {
        if (strlen(domain) > 0 && strlen(nameserver_ip) > 0)
        {
            ngethostbyname(domain, nameserver_ip, output_dir, getQueryTypeIdByName(query_type),
                getTimeout(timeout), getTransportProtocolIdByName(transport_protocol_name),
                dump_to_file);
        }
    }
    else
    {
        auto query_type_id = getQueryTypeIdByName(query_type);
        std::string filename = "";
        if(cmdOptionExists(argv, argv + argc, input_list_arg))
            filename = path_to_input_list;
        if(cmdOptionExists(argv, argv + argc, spoof_list_arg))
            filename = path_to_input_list_with_spoofed_src;

        std::ifstream inputFile(filename.c_str());
        std::deque<Job> VF{};
        auto K = [&](const std::string &domain, const std::string &ns_ip, const std::string spoofed_ip) {
            if(cmdOptionExists(argv, argv + argc, spoof_list_arg))
                return request_dns_and_spoof_src_ip(domain.c_str(), ns_ip.c_str(), spoofed_ip.c_str(), query_type_id);  
            else
                return ngethostbyname(domain.c_str(), ns_ip.c_str(), output_dir, query_type_id,
                    getTimeout(timeout), getTransportProtocolIdByName(transport_protocol_name),
                    dump_to_file);
        };

        while (std::getline(inputFile, line))
        {
            i++;
            spdlog::debug("Iter: " + i);
            std::vector<std::string> x = split(line, '|');
            if (x.size() < 2)
            {
                spdlog::warn("wrong line format, cannot parse");
                continue;
            }

            while (VF.size() > workers)
            {
                auto to = 0;
                auto iter = 0;
                auto now = std::chrono::high_resolution_clock::now();
                for_each(VF.begin(), VF.end(), [&](auto &job) {
                    std::chrono::duration<double> diff = now - job.schedule_time;
                    ++iter;
                    // std::cout<<diff.count()<< " " << getTimeout(timeout)<<std::endl;
                    if (diff.count() > getTimeout(timeout))
                    {
                        std::cout<<"waiting"<<std::endl;
                        to = iter;
                    }
                });
                for_each(VF.begin(), VF.begin() + to, [&](auto &job) {
                    auto result = job.job.get();
                    std::cout<<(result)<< " ";
                    spdlog::debug(result);
                });

                VF.erase(VF.begin(), VF.begin()+to);
            }
            auto now = std::chrono::high_resolution_clock::now();
            if (x.size() == 3)
            {
                VF.push_back(Job{now, std::async(K, x[0], x[1], x[2])});
            }
            else
            {
                VF.push_back(Job{now, std::async(K, x[0], x[1], "")});
            }
        }
        for_each(VF.begin(), VF.end(), [](auto &x) {
            auto result = x.job.get();
            std::cout<<(result)<< " ";
        });
        inputFile.close();
    }
    return 0;
}
