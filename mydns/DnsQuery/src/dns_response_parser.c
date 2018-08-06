#include "../inc/dns_response_parser.h"
#include "utils.h"
#include <string.h>
#include <dns_response_file_writer.h>

void parse_ip(unsigned char *data, FILE *f);

void parse_ptr(unsigned char *data, unsigned short data_len, unsigned char *dns, FILE *f);

void parse_soa(unsigned char *data, unsigned short data_len, unsigned char *dns, FILE *f);

void parse_hinfo(unsigned char *data, unsigned short data_len, FILE *f);

void parse_rrsig(unsigned char *data, unsigned char *, unsigned short data_len, FILE *f);

int getName(unsigned char *data, unsigned short data_len, unsigned char *dns_packet_resp, FILE *f);

void parse_ns(unsigned char *data, unsigned short, unsigned char *dns, FILE *f);

void parse_mx(unsigned char *data, unsigned short data_len, unsigned char *dns_packet_resp, FILE *f);

dns_result parse_txt(unsigned char *data, unsigned short data_len, FILE *f);

void parse_nsec(unsigned char *data, unsigned char *, FILE *f);

void parse_cname(unsigned char *data, unsigned char *dns, FILE *f);

void parse_rp(unsigned char *data, unsigned char *, FILE *f);

void parse_dnskey(unsigned char *, unsigned short, FILE *f);

void parse_afsdb(unsigned char *data, unsigned char *, FILE *f);

void parse_aaaa(unsigned char *data, FILE *f);

void parse_loc(unsigned char *data, FILE *f);

void parse_srv(unsigned char *data, unsigned char *, FILE *f);

dns_result parse_naptr(unsigned char *data, unsigned short, FILE *f);

void parse_default(unsigned char *data, unsigned short data_len, FILE *f);

dns_result ReadName(unsigned char *reader, size_t data_len, unsigned short type, unsigned char *dns, FILE *f)
{
    if (type < 1 || type > 256) {
        return DNS_RESULT_ERR;
    }
    switch (type) {
        case T_A:
            parse_ip(reader, f);
            break;
        case T_NS:
            //printf("datalen: %d", data_len);
            parse_ns(reader, (unsigned short) data_len, dns, f);
            break;
        case T_CNAME:
            parse_cname(reader, dns, f);
            break;
        case T_SOA:
            parse_soa(reader, (unsigned short) data_len, dns, f);
            break;
        case T_PTR:
            parse_ptr(reader, (unsigned short) data_len, dns, f);
            break;
        case T_HINFO:
            parse_hinfo(reader, (unsigned short) data_len, f);
            break;
        case T_MX:
            parse_mx(reader, (unsigned short) data_len, dns, f);
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
            parse_dnskey(reader, (unsigned short) data_len, f);
            break;
        default:
            parse_default(reader, (unsigned short) data_len, f);
            break;
    }
    return DNS_RESULT_OK;
}

unsigned short parse_to_ushort(unsigned char *data)
{
    return (unsigned short) ((*(data) << 8) & 0xFF00) | (*(data + 1) & 0xFF);
}

void parse_default(unsigned char *data, unsigned short data_len, FILE *f)
{
    int i = 0;
    if (data_len == 0) {
        return;
    }

    while (i < data_len) {
        write_raw_data(f, data + i);
        ++i;
    }
    write_endl(f);
}

void parse_dnskey(unsigned char *data, unsigned short data_len, FILE *f)
{
    unsigned short flags = parse_to_ushort(data);
    unsigned char protocol = *(data + 2);
    unsigned char algorithm = *(data + 3);
    int i = 0;
    while (i < data_len - 4) {
        write_raw_data(f, data+i+4);
        ++i;
    }
    write_endl(f);
}

void parse_rp(unsigned char *data, unsigned char *dns, FILE *f)
{
    unsigned char mailbox[1024] = {0};
    unsigned char txt_rr[1024] = {0};

    int x = readSOA(data, dns, mailbox);
    readSOA(data + x + 1, dns, txt_rr);
    write_rp_record(f, mailbox, txt_rr);
}

void parse_afsdb(unsigned char *data, unsigned char *dns, FILE *f)
{
    char write_buffer[1024] = {0};
    unsigned short subtype = parse_to_ushort(data);
    unsigned char hostname[1024] = {0};
    readSOA(data + 2, dns, hostname);
    snprintf(write_buffer, sizeof(write_buffer), "%u %s", subtype, hostname);

    write_string_to_file(f, write_buffer);
    write_endl(f);
}

void parse_aaaa(unsigned char *data, FILE *f)
{
    char ipv6_buffer[1024] = {0};
    snprintf(ipv6_buffer, sizeof(ipv6_buffer), "%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x", *(data), *(data + 1),
            *(data + 2), *(data + 3), *(data + 4), *(data + 5), *(data + 6), *(data + 7), *(data + 8), *(data + 9),
            *(data + 10), *(data + 11), *(data + 12), *(data + 13), *(data + 14), *(data + 15));
    write_string_to_file(f, ipv6_buffer);
    write_endl(f);
}

void parse_loc(unsigned char *data, FILE *f)
{
    char write_buffer[1024] = {0};
    uint8_t size = (uint8_t) *data + 1;
    uint8_t version = (uint8_t) *data;
    uint8_t hor_precision = (uint8_t) *data + 2;
    uint8_t ver_precision = (uint8_t) *data + 3;
    unsigned int latitude = parse_to_uint(data + 4);
    unsigned int longitude = parse_to_uint(data + 8);
    unsigned int altitude = parse_to_uint(data + 12);
    snprintf(write_buffer, sizeof(write_buffer), "%u %u %u %u %u %u %u", version, size, hor_precision, ver_precision, latitude, longitude, altitude);

    write_string_to_file(f, write_buffer);
    write_endl(f);
}

void parse_srv(unsigned char *data, unsigned char *dns, FILE *f)
{
    char write_buffer[1024] = {0};
    unsigned short priority = parse_to_ushort(data);
    unsigned short weight = parse_to_ushort(data + 2);
    unsigned short port = parse_to_ushort(data + 4);
    unsigned char name[512] = {0};
    readSOA(data + 6, dns, name);
    snprintf(write_buffer, sizeof(write_buffer), "%u %u %u %s", priority, weight, port, name);

    write_string_to_file(f, write_buffer);
    write_endl(f);
}

dns_result parse_naptr(unsigned char *data, unsigned short data_len, FILE *f)
{
    unsigned short order = parse_to_ushort(data);
    data += 2;
    unsigned short preference = parse_to_ushort(data);
    data += 2;
    unsigned char flags_length = *(data) & 0xFF;
    ++data;
    unsigned char *txt = NULL;
    unsigned char *txt2 = NULL;

    unsigned char *txt3 = NULL;
    unsigned char *txt4 = NULL;

    int i = 0;

    txt = (unsigned char *) calloc(flags_length + 1, sizeof(unsigned char));
    if (txt == NULL) {
        return DNS_RESULT_NO_MEMORY;
    }
    while (i < flags_length) {
        txt[i++] = *data++;
    }
    txt[i] = '\0';

    unsigned char service_len = *(data) & 0xFF;
    ++data;
    i = 0;
    txt2 = (unsigned char *) calloc(service_len + 1, sizeof(unsigned char));
    if (txt2 == NULL) {
        if (txt != NULL)
            free(txt);
        return DNS_RESULT_NO_MEMORY;
    }
    while (i < service_len) {
        if (txt2 != NULL)
            txt2[i++] = *data++;
    }
    txt2[i] = '\0';

    unsigned char regex_len = *(data) & 0xFF;
    ++data;
    i = 0;
    txt3 = (unsigned char *) calloc(regex_len + 1, sizeof(unsigned char));
    if (txt3 == NULL) {
        if (txt != NULL)
            free(txt);
        if (txt2 != NULL)
            free(txt2);
        return DNS_RESULT_NO_MEMORY;
    }
    while (i < regex_len) {
        txt3[i++] = *data++;
    }
    txt3[i] = '\0';

    i = 0;
    int rep_len = data_len - 5 - 1 - 1 - 1 - service_len - regex_len;
    // ++data;

    txt4 = (unsigned char *) calloc(rep_len + 1, sizeof(unsigned char));
    if (txt4 == NULL) {
        if (txt != NULL)
            free(txt);
        if (txt2 != NULL)
            free(txt2);
        if (txt3 != NULL)
            free(txt3);
        return DNS_RESULT_NO_MEMORY;
    }
    while (i < rep_len) {
        txt4[i++] = *data++;
    }
    txt4[i] = '\0';

    //convert_name(txt4);

    char write_buffer[1024] = {0};
    snprintf(write_buffer, sizeof(write_buffer), "%u %u %d %d %s %s %s %s\n", order, preference, flags_length, service_len, txt, txt2, txt3, txt4);

    write_string_to_file(f, write_buffer);
    write_endl(f);

    if (txt != NULL)
        free(txt);
    if (txt2 != NULL)
        free(txt2);
    if (txt3 != NULL)
        free(txt3);
    if (txt4 != NULL)
        free(txt4);

    return DNS_RESULT_OK;
}

void parse_ip(unsigned char *data, FILE *f)
{
    char write_buffer[1024] = {0};
    snprintf(write_buffer, sizeof(write_buffer), "%d.%d.%d.%d", (int) *data, (int) *(data + 1), (int) *(data + 2), (int) *(data + 3));

    write_string_to_file(f, write_buffer);
    write_endl(f);
}

void parse_ns(unsigned char *data, unsigned short data_len, unsigned char *dns, FILE *f)
{
    getName(data, data_len, dns, f);
    write_endl(f);
}

void parse_ptr(unsigned char *data, unsigned short data_len, unsigned char *dns, FILE *f)
{
    getName(data, data_len, dns, f);// These types all consist of a single domain name
    write_endl(f);
}

void parse_cname(unsigned char *data, unsigned char *dns, FILE *f)
{
    unsigned char name[512] = {0};
    readSOA(data, dns, name);
    write_string_to_file(f, name);
    write_endl(f);
}

void parse_soa(unsigned char *data, unsigned short data_len, unsigned char *dns, FILE *f)
{
    char write_buffer[1024] = {0};
    unsigned char name[1024] = {0};
    unsigned int p = 0;

    p = readSOA(data, dns, name);
    int offset = snprintf(write_buffer, sizeof(write_buffer), " %s ", name);

    memset(name, 0, 1024);
    p = readSOA(data + p + 1, dns, name);
    write_string_to_file(f, name);

    unsigned int serial_no = parse_to_uint(data + p + 3);
    unsigned int refresh = parse_to_uint(data + p + 7);
    unsigned int retry = parse_to_uint(data + p + 11);
    unsigned int expire = parse_to_uint(data + p + 15);
    unsigned int min_ttl = parse_to_uint(data + p + 19);
    snprintf(write_buffer+offset, sizeof(write_buffer)-offset-1, " %u %u %u %u %u", serial_no, refresh, retry, expire, min_ttl);

    write_string_to_file(f, name);
    write_endl(f);
}

void parse_hinfo(unsigned char *data, unsigned short data_len, FILE *f)
{
    char write_buffer[1024] = {0};
    int len_cpu = (int) *data;
    int len_os = (int) *(data + len_cpu + 1);

    int offset = snprintf(write_buffer, sizeof(write_buffer), "cpu len: %d os len: %d ", len_cpu, len_os);
    unsigned char algorithm = *(data + 3);
    int i = 0;
    while (i < data_len) {
        offset += snprintf(write_buffer + offset, sizeof(write_buffer) - offset - 1, "%02x", *(data + i));
        ++i;
    }

    write_string_to_file(f, write_buffer);
    write_endl(f);
}

void parse_mx(unsigned char *data, unsigned short data_len, unsigned char *dns_packet_resp, FILE *f)
{
    unsigned short preference = ((*(data) << 8) & 0xFF00) | (*(data + 1) & 0xFF);
    getName(data + 2, data_len, dns_packet_resp, f);
    write_endl(f);
}

dns_result parse_txt(unsigned char *data, unsigned short data_len, FILE *f)
{
    unsigned int i = 0;
    unsigned char *txt = NULL;

    txt = (unsigned char *) calloc(data_len, sizeof(unsigned char));
    if (txt == NULL) {
        return DNS_RESULT_NO_MEMORY;
    }
    data++;
    while (i < data_len - 1) {
        txt[i++] = *data++;
    }

    write_string_to_file(f, txt);
    write_endl(f);
    if (txt != NULL)
        free(txt);
    return DNS_RESULT_OK;
}

void parse_rrsig(unsigned char *data, unsigned char *dns, unsigned short data_len, FILE *f)
{
    char write_buffer[1024] = {0};

    //TODO CONVERT TIME
    unsigned short type = ((*(data) << 8) & 0xFF00) | (*(data + 1) & 0xFF);
    unsigned char algorithm = *(data + 2) & 0xFF;
    unsigned char labels = *(data + 3) & 0xFF;
    unsigned int ttl = parse_to_uint(data + 4);
    unsigned int timestamp_exp = parse_to_uint(data + 8);
    unsigned int timestamp_inc = parse_to_uint(data + 12);
    unsigned short key_tag = type = ((*(data + 16) << 8) & 0xFF00) | (*(data + 17) & 0xFF);
    unsigned char name[1024] = {0};
    unsigned int p = 0;

    p = readSOA(data + 18, dns, name);

    int offset = snprintf(write_buffer, sizeof(write_buffer), "%u %u %u %u %u %u %u %s ", type, algorithm, labels, ttl, timestamp_exp, timestamp_inc, key_tag, name);
    int i = 0;
    while (i < data_len - p - 19) {
        offset+=snprintf(write_buffer+offset, sizeof(write_buffer) - offset, "%02x", *(data + p + i + 19));
        ++i;
    }
    write_string_to_file(f, write_buffer);
    write_endl(f);
}

void parse_nsec(unsigned char *data, unsigned char *dns, FILE *f)
{
    unsigned char name[512] = {0};
    readSOA(data, dns, name);
    write_string_to_file(f, name);
    write_endl(f);
}

int getName(unsigned char *data, unsigned short data_len, unsigned char *dns_packet_resp, FILE *f)
{
    unsigned char name[1024] = {0};
    unsigned int i = 0, j = 0, p = 0;

    p = readString(data, data_len, dns_packet_resp, name);
    //printf("after readString");

    //now convert 3www6google3com0 to www.google.com
    for (i = 0; i < (int) strlen((const char *) name); i++) {
        p = name[i];
        for (j = 0; j < (int) p; j++) {
            name[i] = name[i + 1];
            i = i + 1;
        }
        name[i] = '.';
    }
    if (i > 0) {
        name[i - 1] = '\0'; //remove the last dot
    }

    write_string_to_file(f, name);
    return strlen((char *) name);
}
