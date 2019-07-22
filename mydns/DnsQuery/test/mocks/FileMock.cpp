//
// Created by mskwarek on 19.06.18.
//

#include "FileMock.h"

CMOCK_MOCK_FUNCTION2(FileMock, fopen, FILE *(const char *, const char *))
CMOCK_MOCK_FUNCTION1(FileMock, fclose, int(FILE *))
