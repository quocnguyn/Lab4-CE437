
#ifndef DIAGNOSTICDEFINE_H_
#define DIAGNOSTICDEFINE_H_

#include <stdint.h>
#include "main.h"

#ifdef 	ECUNode
	#include "ECU_Diagnostic/ECU_Diagnostic.h"
#elif 	TesterNode
	#include "Tester_Diagnostic/Tester_Diagnostic.h"
#endif
#include "CanTP.h"

#define ECU_ID						0x7A2
#define TESTER_ID					0x712

#ifdef 	TesterNode
	#define	SEND_ID 				ECU_ID
	#define RECEIVE_ID 				TESTER_ID
#elif 	ECUNode
	#define	SEND_ID 				TESTER_ID
	#define RECEIVE_ID 				ECU_ID
#endif

#define ReadDataByID_RequestSID 	0x22
#define WriteDataByID_RequestSID	0x2E
#define SecurityAccess_RequestSID	0x27

#define ReadData_CanID_DID			0x0123
#define WriteData_CanID_DID			0x0123

#define SecurityAccess_ReqSeedID	0x01
#define SecurityAccess_SendKeyID	0x02

#define NegResp_InvalidLen					0x13
#define NegResp_SecurityAccess_InvalidKey	0x35
#define NegResp_DID_notSupport				0x31

#define Get_Positive_RespID(reqID)			((uint16_t)reqID + 0x40)
#define Get_Recv_DID(DID_H,DID_L)			((uint16_t)DID_H << 8 | (DID_L & 0xFF))

typedef void (*ServiceHandler)();

void calculateKeys(uint8_t*, uint8_t*);
uint8_t diagnosticService_Init();
extern ServiceHandler serviceHandler;

#endif
