/*
 * can_app.h
 *
 *  Created on: 23-Jul-2022
 *      Author: Prabhu
 */

#ifndef CAN_APP_H_
#define CAN_APP_H_


void can_data_push(void);
extern void uart_send_received_can_data(void);

extern volatile uint8_t can_queue_timeout_vu8;

extern volatile uint8_t can_queue_timeout_check_vu8;
extern uint16_t can_queue_check_counter_16;


#endif /* CAN_APP_H_ */
