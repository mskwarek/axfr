//
// Created by mskwarek on 06.08.18.
//

#include "dns_response_file_writer.h"

size_t write_raw_data(char *output_buf, size_t output_buf_size, const char *data)
{
    if (data != NULL)
        return snprintf(output_buf, output_buf_size, "%02x", *(data));
    
    return 0;
}

size_t write_endl(char *output_buf, size_t output_buf_size)
{
    return snprintf(output_buf, output_buf_size, "\n");
}

size_t write_rp_record(char *output_buf, size_t output_buf_size, const char *mailbox, const char *txt_rr)
{
    return snprintf(output_buf, output_buf_size, "%s %s\n", mailbox, txt_rr);
}

size_t write_string_to_file(char *output_buf, size_t output_buf_size, const char *string)
{
    return snprintf(output_buf, output_buf_size, "%s", string);
}
