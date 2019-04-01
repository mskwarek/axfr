//
// Created by mskwarek on 06.08.18.
//

#ifndef AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H
#define AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

void write_raw_data(FILE *f, const char *data);
void write_endl(FILE *f);
void write_rp_record(FILE *f, const char *, const char *);
void write_string_to_file(FILE *f, const char *string);

#ifdef __cplusplus
}
#endif

#endif // AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H
