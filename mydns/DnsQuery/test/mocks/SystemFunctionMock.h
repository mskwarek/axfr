//
// Created by mskwarek on 12.07.18.
//

#ifndef AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
#define AXFRSCANNER_SYSTEMFUNCTIONMOCK_H

#include <cmock/cmock.h>
#include "../../../SystemFunctionProxy/inc/proxy_functions.h"

DECLARE_FUNCTION_MOCK3(SocketFcntlMock, int_fcntl, int(int, int, long));
DECLARE_FUNCTION_MOCK4(PrintToFilelMock, sys_print_domain_info_to_file, void(FILE*, const char*, int, int));

#endif //AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
