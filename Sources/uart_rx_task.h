/*
 * uart_rx_task.h
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */

#ifndef UART_RX_TASK_H_
#define UART_RX_TASK_H_

extern void uart_rx_task_v(void *parameter);

extern  void bootloader_app_uart_cmd_ack_send_v(U8 cmd,U8 cmd_ack_sts);

#endif /* UART_RX_TASK_H_ */
