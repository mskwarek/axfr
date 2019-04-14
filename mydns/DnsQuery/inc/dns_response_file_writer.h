//
// Created by mskwarek on 06.08.18.
//

#ifndef AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H
#define AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

size_t write_raw_data(char *output_buf, size_t output_buf_size, const char *data);
size_t write_endl(char *output_buf, size_t output_buf_size);
size_t write_rp_record(char *output_buf, size_t output_buf_size, const char *, const char *);
size_t write_string_to_file(char *output_buf, size_t output_buf_size, const char *string);

#ifdef __cplusplus
}
#endif

#endif // AXFRSCANNER_DNS_RESPONSE_FILE_WRITER_H
