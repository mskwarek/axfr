//
// Created by mskwarek on 06.08.18.
//

#ifndef AXFRSCANNER_DNSRESPONSEFILEWRITER_H
#define AXFRSCANNER_DNSRESPONSEFILEWRITER_H

#include "cmock/cmock.h"
#include "dns_response_file_writer.h"

DECLARE_FUNCTION_MOCK2(WriteRawDataMock, write_raw_data, void(FILE *, const char *));
DECLARE_FUNCTION_MOCK1(WriteEndlMock, write_endl, void(FILE *));
DECLARE_FUNCTION_MOCK3(
    WriteRpRecordMock, write_rp_record, void(FILE *, const char *, const char *));
DECLARE_FUNCTION_MOCK2(WriteStringToFileMock, write_string_to_file, void(FILE *, const char *));

#endif // AXFRSCANNER_DNSRESPONSEFILEWRITER_H

