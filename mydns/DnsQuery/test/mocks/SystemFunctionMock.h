//
// Created by mskwarek on 12.07.18.
//

#ifndef AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
#define AXFRSCANNER_SYSTEMFUNCTIONMOCK_H

#include <cmock/cmock.h>
#include "proxy_functions.h"
#include <stdlib.h>

class SystemFunctionMock : public CMockMocker<SystemFunctionMock>
{
public:
    MOCK_METHOD3(int_fcntl, int(int, int, long));
    MOCK_METHOD5(sys_print_domain_info_to_file, int(char*, size_t, const char *, int, int));
    MOCK_METHOD3(sys_print_buffer_to_file, int(const char*, size_t, const char*));
};

#endif // AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
