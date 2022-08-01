/*
 * uart_tx_task.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */


#include "operating_system.h"
#include "uart_rx_task.h"
#include "can_app.h"
#include "uart_state_machine.h"
#include "can.h"

operating_system_uart_tx_queue_tst queue_received_uart_send_gst;

void uart_tx_task_v(void *parameter)
{
	(void)(parameter);
	while(1)
	{
		if (xQueueReceive(os_uart_tx_queue_handler_ge, &queue_received_uart_send_gst, portMAX_DELAY))
		{
			switch(queue_received_uart_send_gst.event_e)
			{
/***********************************************************************************************************/
				case UART_TX_HEART_BEAT_SEND:
				{
					if((last_uart_command_ack_pending == false)
						&& (last_can_command_ack_pending == false))
					{
						send_heartbeat();
					}

				}
				break;
/***********************************************************************************************************/
				case UART_TX_CAN_DATA_SEND:
				{
					uart_send_received_can_data();
				}
				break;
/************************************************************************************************************/
				default:
				{

				}
				break;
/*************************************************************************************************************/
			}

		}
	}
}
