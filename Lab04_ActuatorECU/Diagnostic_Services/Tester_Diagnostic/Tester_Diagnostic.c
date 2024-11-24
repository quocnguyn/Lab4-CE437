#include "DiagnosticDefine.h"
#include "main.h"
#include "Tester_Diagnostic.h"
#include "CanTP.h"
#include <stdio.h>
#include <string.h>

extern uint8_t write_data[];

void request(){
	readDataByID_RequestService(ReadData_CanID_DID);
	HAL_Delay(2000);
	securityAccess_RequestService();
	HAL_Delay(2000);
	writeDataByID_RequestService(
			WriteData_CanID_DID,
			write_data,
			LENGTH(write_data)
	);
	HAL_Delay(2000);
}

uint8_t Tester_Init() {
	serviceHandler = &request;
	HAL_ERR(CanTP_Init(SEND_ID, RECEIVE_ID));
	return HAL_OK;
}

uint8_t readDataByID_RequestService(uint16_t DID) {
	uint8_t 	send_buffer[3];
	uint16_t 	receive_length = 20;
	uint8_t 	receive_buffer[receive_length];

	// send request to ECU to use WriteDataByID service
	send_buffer[0] = ReadDataByID_RequestSID;
	send_buffer[1] = DID >> 8;
	send_buffer[2] = DID & 0xFF;
	HAL_ERR(CanTP_Transmit(send_buffer, LENGTH(send_buffer)));

	// If received message is positive response with correct DID,
	// copy received data to a buffer
	HAL_ERR(CanTP_Receive(receive_buffer, &receive_length, 500));
	uint8_t response_SID = receive_buffer[0];
	if (response_SID == Get_Positive_RespID(ReadDataByID_RequestSID)
			&&
		DID == Get_Recv_DID(receive_buffer[1], receive_buffer[2])
	){
		uint16_t read_data_length = receive_length - 3;
		uint8_t read_data_buffer[read_data_length];
		memcpy(read_data_buffer, receive_buffer + 3, read_data_length);
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t writeDataByID_RequestService(uint16_t DID, uint8_t *data, uint8_t length) {
	uint8_t 	send_buffer[length + 3];
	uint16_t 	receive_length = 20;
	uint8_t 	receive_buffer[receive_length];

	// send request to ECU to use WriteDataByID service
	send_buffer[0] = WriteDataByID_RequestSID;
	send_buffer[1] = DID >> 8;
	send_buffer[2] = DID & 0xFF;
	memcpy(send_buffer + 3, data, length);
	HAL_ERR(CanTP_Transmit(send_buffer, length + 3));

	// check if received message is a negative response
	HAL_ERR(CanTP_Receive(receive_buffer, &receive_length, 500));
	uint8_t response_SID = receive_buffer[0];
	if (response_SID == Get_Positive_RespID(WriteDataByID_RequestSID)) {
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t waitForECUVerification(){
	uint8_t receive_buffer[3];
	uint16_t receive_buffer_length = LENGTH(receive_buffer);
	HAL_ERR(CanTP_Receive(receive_buffer, &receive_buffer_length, 500));

	// check if received message is positive response
	// with SendKeys sub function
	uint8_t recv_SID = receive_buffer[0];
	uint8_t recv_SF  = receive_buffer[1];
	if(
		recv_SID == Get_Positive_RespID(SecurityAccess_RequestSID)
			&&
		recv_SF == SecurityAccess_SendKeyID
	){
		return HAL_OK;
	}
	return HAL_ERROR;
}

uint8_t securityAccess_RequestService() {
	uint8_t req_seeds_buff[] = { SecurityAccess_RequestSID, SecurityAccess_ReqSeedID };
	uint8_t recv_seeds_buff[6];
	uint16_t recv_seeds_buff_length = LENGTH(req_seeds_buff);

	// request seeds from ECU
	HAL_ERR(CanTP_Transmit(req_seeds_buff, recv_seeds_buff_length));

	// check if received seeds is valid
	HAL_ERR(CanTP_Receive(recv_seeds_buff, &recv_seeds_buff_length, 500));
	uint8_t RespSID = recv_seeds_buff[0];
	uint8_t RespSF = recv_seeds_buff[1];
	if (RespSID != Get_Positive_RespID(SecurityAccess_RequestSID)
			|| RespSF != SecurityAccess_ReqSeedID) {
		return HAL_ERROR;
	}

	// calculate keys
	uint8_t seeds[4];
	memcpy(seeds, recv_seeds_buff + 2, 4);
	uint8_t keys[16];
	calculateKeys(keys, seeds);

	// send keys
	uint8_t send_buffer[18];
	send_buffer[0] = SecurityAccess_RequestSID;
	send_buffer[1] = SecurityAccess_SendKeyID;
	memcpy(send_buffer + 2, keys, LENGTH(keys));
	HAL_ERR(CanTP_Transmit(send_buffer, LENGTH(send_buffer)));

	HAL_ERR(waitForECUVerification());
	return HAL_OK;
}
