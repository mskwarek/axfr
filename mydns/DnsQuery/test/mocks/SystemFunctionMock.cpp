//
// Created by mskwarek on 12.07.18.
//

#include "SystemFunctionMock.h"

IMPLEMENT_FUNCTION_MOCK3(SocketFcntlMock, int_fcntl, int (int, int, long));
IMPLEMENT_FUNCTION_MOCK4(PrintToFilelMock, sys_print_domain_info_to_file, void(FILE*, const char*, int, int));
