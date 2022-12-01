/*
 * can_tx_task.h
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */

#ifndef CAN_TX_TASK_H_
#define CAN_TX_TASK_H_

extern uint8_t jmp_cmd_sent_u8;

extern void can_comm_tx_task_v(void *parameter);

#endif /* CAN_TX_TASK_H_ */
