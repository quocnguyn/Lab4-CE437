#ifndef CANTP_H_
#define CANTP_H_

#include <stdint.h>
#define CANTP_BUFF_SIZE			100

uint8_t CanTP_Init(uint16_t, uint16_t);
uint8_t CanTP_Transmit(uint8_t*, uint16_t);
uint8_t CanTP_Receive(uint8_t* ,uint16_t*, uint32_t);

void CanTP_RcvCallback();

#endif

