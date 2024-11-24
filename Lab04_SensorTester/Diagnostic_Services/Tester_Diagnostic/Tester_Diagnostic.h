#ifndef TESTER_DIAGNOSTIC_H_
#define TESTER_DIAGNOSTIC_H_

#include <stdint.h>
#include "main.h"

uint8_t Tester_Init();

uint8_t readDataByID_RequestService(uint16_t);
uint8_t writeDataByID_RequestService(uint16_t, uint8_t*, uint8_t);
uint8_t securityAccess_RequestService();

#endif
