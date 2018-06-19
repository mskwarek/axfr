//
// Created by mskwarek on 19.06.18.
//

#include "FileMock.h"

IMPLEMENT_FUNCTION_MOCK2(FileMock, fopen, FILE *(const char *, const char * ));
IMPLEMENT_FUNCTION_MOCK1(FileCloseMock, fclose, int(FILE* ));

