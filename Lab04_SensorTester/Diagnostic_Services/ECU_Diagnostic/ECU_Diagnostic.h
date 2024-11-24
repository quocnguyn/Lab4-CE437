#ifndef ECU_DIAGNOSTIC_H_
#define ECU_DIAGNOSTIC_H_

#include "main.h"

typedef enum {
	UNLOCK,
	LOCK
} SecurityState;

uint8_t ECU_Init();

uint8_t readDataByID_ResponseService(uint8_t[], uint16_t);
uint8_t writeDataByID_ResponseService(uint8_t[], uint16_t);
uint8_t securityAccess_ResponseService(uint8_t[], uint16_t);

#endif

