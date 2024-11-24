#include "ECU_Diagnostic.h"
#include "DiagnosticDefine.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "CanTP.h"

extern UART_HandleTypeDef huart1;
static SecurityState security_state = LOCK;

static void generateSeeds(uint8_t seeds[]) {
	srand((unsigned int)time(NULL));  // initialize random seed

	for(size_t i = 0; i < 4; i++){
		seeds[i] = rand() & 0xFF;
	}
}


static uint8_t checkKey(uint8_t received_keys[], uint8_t calculated_keys[]) {
	for (int i = 0; i < 16; i++) {
		if (received_keys[i] != calculated_keys[i])
			return 0;
	}
	return 1;
}


inline static void changeState(SecurityState state) {
	if (security_state == UNLOCK)
		return;

	HAL_GPIO_WritePin(
		LEDIn_GPIO_Port,
		LEDIn_Pin,
		(state ? GPIO_PIN_SET : GPIO_PIN_RESET)
	);
	security_state = state;
}

void response() {
	uint8_t received_data[20] = {0};
	uint16_t received_data_length = 20;
	uint8_t recv_state = CanTP_Receive(received_data, &received_data_length, 500);

	if (recv_state != HAL_OK){
		return;
	}

	int recv_SID = received_data[0];
	switch (recv_SID) {
		case ReadDataByID_RequestSID:
			readDataByID_ResponseService(received_data, received_data_length);
			break;
		case WriteDataByID_RequestSID:
			writeDataByID_ResponseService(received_data, received_data_length);
			break;
		case SecurityAccess_RequestSID:
			securityAccess_ResponseService(received_data, received_data_length);
			break;
		default:
			break;
	}
}

uint8_t ECU_Init() {
	HAL_ERR(CanTP_Init(SEND_ID, RECEIVE_ID));
	serviceHandler = &response;
	return HAL_OK;
}


inline static uint8_t sendNegativeResponse(uint8_t SID, uint8_t exception_ID){
	uint8_t neg_response[3] = {0x7F, SID, exception_ID};
	return CanTP_Transmit(neg_response, LENGTH(neg_response));
}


uint8_t readDataByID_ResponseService(uint8_t data[], uint16_t length) {
	uint16_t recvDID = Get_Recv_DID(data[1], data[2]);
	if (recvDID != ReadData_CanID_DID) {
		HAL_ERR(sendNegativeResponse(
			ReadDataByID_RequestSID,
			NegResp_InvalidLen
		));
		HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
		return HAL_OK;
	}

	uint8_t pos_response[5] = {
		Get_Positive_RespID(ReadDataByID_RequestSID),
		data[1],
		data[2], 0, 0
	};
	pos_response[3] = RECEIVE_ID >> 8;
	pos_response[4] = RECEIVE_ID & 0xFF;

	HAL_ERR(CanTP_Transmit(pos_response, 5));

	HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_RESET);
	return HAL_OK;
}


uint8_t writeDataByID_ResponseService(uint8_t *data, uint16_t length) {
	if (security_state == LOCK) {
		return HAL_OK;
	}

	// If data length is greater than 4, send negative response to the tester
	if (length < 4) {
		HAL_ERR(sendNegativeResponse(
			WriteDataByID_RequestSID,
			NegResp_InvalidLen
		));
		HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
		return HAL_OK;
	}

	// If the MCU get invalid DID, send negative response back to the tester
	uint16_t recvDID = Get_Recv_DID(data[1], data[2]);
	if (recvDID != WriteData_CanID_DID) {
		HAL_ERR(sendNegativeResponse(
			WriteDataByID_RequestSID,
			NegResp_DID_notSupport
		));
		HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
		return HAL_OK;
	}

	// If the received message is valid, send positive response to the tester
	uint8_t pos_response[] = { Get_Positive_RespID(WriteDataByID_RequestSID) };
	HAL_ERR(CanTP_Transmit(pos_response, LENGTH(pos_response)));
	return HAL_OK;
}


uint8_t securityAccess_SendSeeds(uint8_t* seeds){
	uint8_t pos_response[6] = {
		Get_Positive_RespID(SecurityAccess_RequestSID),
		SecurityAccess_ReqSeedID, 0, 0, 0, 0
	};
	memcpy(pos_response + 2, seeds, 4);
	return CanTP_Transmit(pos_response, LENGTH(pos_response));
}


void notify(const uint8_t* message){
	uint8_t uart_buffer[30] = "";
	uint8_t msg_length = sprintf(uart_buffer, message);
	HAL_UART_Transmit(&huart1, uart_buffer, msg_length, 200);
}



uint8_t securityAccess_ResponseService(uint8_t *data, uint16_t length) {
	uint8_t recv_SF = data[1]; // received sub function ID

	if (recv_SF != SecurityAccess_ReqSeedID) {
		return HAL_OK;
	}

	uint8_t seeds[4];
	generateSeeds(seeds);
	HAL_ERR(securityAccess_SendSeeds(seeds));

	uint16_t recv_buffer_length = 18;
	uint8_t recv_buffer[18] = {0};
	memset(recv_buffer, 0, recv_buffer_length);

	HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
	uint8_t keys[16] = {0};
	calculateKeys(keys, seeds);

	// receive keys from Tester
	HAL_ERR(CanTP_Receive(recv_buffer, &recv_buffer_length, 1500));

	// keys received message is printed by UART
	notify("RKS\r\n");
	// toggle LED to notify
	HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);

	uint8_t recv_SID = recv_buffer[0];
	recv_SF = recv_buffer[1];
	if (recv_SID != SecurityAccess_RequestSID) {
		HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);

		uint8_t message[30] = "";
		sprintf(
			message,
			"WSID: %x,%x,%x\r\n",
			recv_buffer[0], recv_buffer[1], recv_buffer[2]
		);
		notify(message);
		HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
		return HAL_OK;
	} else if (recv_SF != SecurityAccess_SendKeyID) {
		HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
		notify("WSF\r\n");
		HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
		return HAL_OK;
	}

	uint8_t received_keys[16] = {0};
	memcpy(received_keys, recv_buffer + 2, 16);
	if (!checkKey(received_keys, keys)) {
		HAL_ERR(sendNegativeResponse(
			SecurityAccess_RequestSID,
			NegResp_SecurityAccess_InvalidKey
		));
		HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
		notify("WK\r\n");
		return HAL_OK;
	}
	HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);

	changeState(UNLOCK);
	uint8_t pos_response[2] = {
		Get_Positive_RespID(SecurityAccess_RequestSID),
		SecurityAccess_SendKeyID
	};
	HAL_ERR(CanTP_Transmit(pos_response, LENGTH(pos_response)));
	return HAL_OK;
}
