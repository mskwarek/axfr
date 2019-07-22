//
// Created by mskwarek on 06.08.18.
//

#ifndef AXFRSCANNER_DNSRESPONSEFILEWRITERMOCK_H
#define AXFRSCANNER_DNSRESPONSEFILEWRITERMOCK_H

#include <cmock/cmock.h>
#include "dns_response_file_writer.h"

class WriteMock : public CMockMocker<WriteMock>
{
public:
    MOCK_METHOD3(write_raw_data, size_t(char *, size_t, const char *));
    MOCK_METHOD2(write_endl, size_t(char *, size_t));
    MOCK_METHOD4(write_rp_record, size_t(char *, size_t, const char *, const char *));
    MOCK_METHOD3(write_string_to_file, size_t(char *, size_t, const char *));
};

#endif // AXFRSCANNER_DNSRESPONSEFILEWRITERMOCK_H
