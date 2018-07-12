//
// Created by mskwarek on 12.07.18.
//

#ifndef AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
#define AXFRSCANNER_SYSTEMFUNCTIONMOCK_H

#include <cmock/cmock.h>

DECLARE_FUNCTION_MOCK3(SocketFcntlMock, int_fcntl, int(int, int, long));

#endif //AXFRSCANNER_SYSTEMFUNCTIONMOCK_H
