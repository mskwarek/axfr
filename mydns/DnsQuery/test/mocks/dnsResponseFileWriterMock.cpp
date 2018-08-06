//
// Created by mskwarek on 06.08.18.
//

#include "dnsResponseFileWriterMock.h"

IMPLEMENT_FUNCTION_MOCK2(WriteRawDataMock, write_raw_data, void(FILE*, const char*));
IMPLEMENT_FUNCTION_MOCK1(WriteEndlMock, write_endl, void(FILE*));
IMPLEMENT_FUNCTION_MOCK3(WriteRpRecordMock, write_rp_record, void(FILE *f, const char*, const char*));
IMPLEMENT_FUNCTION_MOCK2(WriteStringToFileMock, write_string_to_file, void(FILE *f, const char *));