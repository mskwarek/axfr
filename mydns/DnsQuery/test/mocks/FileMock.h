//
// Created by mskwarek on 19.06.18.
//

#ifndef AXFRSCANNER_FILEMOCK_H
#define AXFRSCANNER_FILEMOCK_H

#include "cmock/cmock.h"
#include "stdio.h"

DECLARE_FUNCTION_MOCK2(FileMock, fopen, FILE*(const char *, const char * ));
DECLARE_FUNCTION_MOCK1(FileCloseMock, fclose, int(FILE* ));

#endif //AXFRSCANNER_FILEMOCK_H
