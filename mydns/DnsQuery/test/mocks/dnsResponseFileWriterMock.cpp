//
// Created by mskwarek on 06.08.18.
//

#include "dnsResponseFileWriterMock.h"

CMOCK_MOCK_FUNCTION3(WriteMock, write_raw_data, size_t(char *, size_t, const char *))
CMOCK_MOCK_FUNCTION2(WriteMock, write_endl, size_t(char *, size_t))
CMOCK_MOCK_FUNCTION4(WriteMock, write_rp_record, size_t(char *, size_t, const char *, const char *))
CMOCK_MOCK_FUNCTION3(WriteMock, write_string_to_file, size_t(char *, size_t, const char *))
