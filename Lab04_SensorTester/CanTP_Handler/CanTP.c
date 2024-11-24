#include "CanTP.h"
#include "main.h"
#include "isotp.h"

extern CAN_HandleTypeDef hcan;
extern UART_HandleTypeDef huart1;
IsoTpLink isoTP;

inline static void CAN_Filter_Init(CAN_FilterTypeDef* can_filter, uint16_t receive_ID){
	can_filter->FilterActivation = CAN_FILTER_ENABLE;
	can_filter->FilterBank = 0;
	can_filter->FilterFIFOAssignment = CAN_FILTER_FIFO0;
	can_filter->FilterMode = CAN_FILTERMODE_IDLIST;
	can_filter->FilterScale = CAN_FILTERSCALE_16BIT;
	can_filter->FilterIdHigh = receive_ID << 5;
}

uint8_t CanTP_Init(uint16_t send_ID, uint16_t receive_ID) {
	// initialize CAN filter
	CAN_FilterTypeDef can_filter;
	CAN_Filter_Init(&can_filter, receive_ID);

	// configure CAN filter
	HAL_ERR(HAL_CAN_ConfigFilter(&hcan, &can_filter));

	// enable FIFO0 interrupt
	HAL_ERR(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING));

	// start CAN
	HAL_ERR(HAL_CAN_Start(&hcan));

	// initialize an IsoTpLink
	uint8_t send_buffer[CANTP_BUFF_SIZE];
	uint8_t received_buffer[CANTP_BUFF_SIZE];
	isotp_init_link(
		&isoTP, send_ID,
		send_buffer, CANTP_BUFF_SIZE,
		received_buffer, CANTP_BUFF_SIZE
	);
	return HAL_OK;
}

uint8_t CanTP_Transmit(uint8_t *data, uint16_t length) {
	// send a message
	HAL_ERR(isotp_send(&isoTP, data, length));
	while (isoTP.send_status == ISOTP_SEND_STATUS_INPROGRESS) {
		isotp_poll(&isoTP);
	}

	// return the sent message status
	if (ISOTP_SEND_STATUS_IDLE != isoTP.send_status) {
		HAL_GPIO_WritePin(LEDG_GPIO_Port, LEDG_Pin, GPIO_PIN_SET);
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint8_t CanTP_Receive(uint8_t *data, uint16_t *length, uint32_t timeout) {
	// wait for the message to be received until reaching the timeout
	uint32_t start_time = HAL_GetTick();
	while ((HAL_GetTick() - start_time) < timeout) {
		isotp_poll(&isoTP);
		if (isoTP.receive_status == ISOTP_RECEIVE_STATUS_FULL) {
			break;
		}
	}

	// If the message is received successfully, copy receive buffer to
	// the data container
	if (ISOTP_RECEIVE_STATUS_FULL == isoTP.receive_status) {
		isotp_receive(&isoTP, data, *length, length);
		uint16_t copylen = isoTP.receive_size;
		if (copylen > *length) {
			copylen = *length;
		}
		for (int i = 0; i < copylen; i++) {
			data[i] = isoTP.receive_buffer[i];
		}
		return HAL_OK;
	}
	HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_SET);
	return HAL_TIMEOUT;
}

void CanTP_RcvCallback() {
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8] = {0};
	HAL_CAN_GetRxMessage(&hcan, CAN_FILTER_FIFO0, &RxHeader, RxData);
	isotp_on_can_message(&isoTP, RxData, RxHeader.DLC);
}
