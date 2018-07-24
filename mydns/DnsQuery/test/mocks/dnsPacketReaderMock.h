//
// Created by mskwarek on 24.07.18.
//

#ifndef AXFRSCANNER_DNSPACKETREADERMOCK_H
#define AXFRSCANNER_DNSPACKETREADERMOCK_H

#include "cmock/cmock.h"
#include "dns_received_packet_reader.h"

DECLARE_FUNCTION_MOCK2(IsLackOfMemoryMock, is_lack_of_memory, int(unsigned char*, int));

#endif //AXFRSCANNER_DNSPACKETREADERMOCK_H
