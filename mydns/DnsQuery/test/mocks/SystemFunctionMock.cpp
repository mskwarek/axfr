//
// Created by mskwarek on 12.07.18.
//

#include "SystemFunctionMock.h"

CMOCK_MOCK_FUNCTION3(SystemFunctionMock, int_fcntl, int(int, int, long))
CMOCK_MOCK_FUNCTION5(SystemFunctionMock, sys_print_domain_info_to_file, int(char*, size_t, const char *, int, int))
CMOCK_MOCK_FUNCTION3(SystemFunctionMock, sys_print_buffer_to_file, int(const char*, size_t, const char*))
