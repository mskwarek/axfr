//
// Created by mskwarek on 06.08.18.
//

#include "dns_response_file_writer.h"

void write_raw_data(FILE *f, const char *data)
{
    if (data != NULL)
        fprintf(f, "%02x", *(data));
}

void write_endl(FILE *f)
{
    fprintf(f, "\n");
}

void write_rp_record(FILE *f, const char *mailbox, const char *txt_rr)
{
    fprintf(f, "%s %s\n", mailbox, txt_rr);
}

void write_string_to_file(FILE *f, const char *string)
{
    fprintf(f, "%s", string);
}