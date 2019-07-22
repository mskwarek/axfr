//
// Created by mskwarek on 19.06.18.
//

#ifndef AXFRSCANNER_FILEMOCK_H
#define AXFRSCANNER_FILEMOCK_H

#include <cmock/cmock.h>
#include "stdio.h"

class FileMock : public CMockMocker<FileMock>
{
public:
    MOCK_METHOD2(fopen, FILE *(const char *, const char *));
    MOCK_METHOD1(fclose, int(FILE *));
};

#endif // AXFRSCANNER_FILEMOCK_H

