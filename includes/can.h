/*
 * can.h
 *
 *  Created on: Jun 7, 2022
 *      Author: jerin
 */

#ifndef CAN_H_
#define CAN_H_

#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "can_pal.h"
#include "can_pal1.h"
#include "can_queue.h"
#include "common_header.h"

#define CAN_TX_MAILBOX0 		(0U)
#define CAN_TX_MAILBOX1 		(1U)

#define CAN_RX_MAILBOX0 		(2U)
#define CAN_RX_MAILBOX1 		(3U)
#define CAN_RX_MAILBOX2 		(4U)
#define CAN_RX_MAILBOX3 		(5U)

typedef enum can_tx_msg_list_e_tag
{
	CAN_DATA1 =1,
	CAN_DATA2,
	CAN_DATA3,
	CAN_DATA4,
	CAN_DATA5,
	CAN_DATA6,
	CAN_DATA7,
	CAN_DATA8,
	CAN_DATA9,
	CAN_DATA10,
	MAX_CAN_DATA
}can_tx_msg_list_e;

void can_init(void);
void can_receive_enable(void);
void can_rx_task(void * param);
void can_uart_parser(can_message_t *tx_msg);

void can_tx_task(void *param);
bool filter_can_msg(uint32_t ext_id);
void can_tx_message(can_message_t *can_tx_msg);
extern can_message_t  can_rx_tx_msg_st;
extern volatile uint8_t can_rx_msg_received_vu8;

extern uint8_t last_can_command_ack_pending;
#endif /* CAN_H_ */
