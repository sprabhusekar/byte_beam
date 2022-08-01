/*

 * uart_rx_task.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */

#include "operating_system.h"
#include "common_header.h"
#include "uart_state_machine.h"
#include "can.h"
operating_system_uart_rx_queue_tst queue_received_uart_ack_gst;

void uart_rx_task_v(void *parameter)
{
	(void)(parameter);

	while(1)
	{
		if (xQueueReceive(os_uart_rx_queue_handler_ge, &queue_received_uart_ack_gst, portMAX_DELAY))
		{
			switch(queue_received_uart_ack_gst.event_e)
			{
/*********************************************************************************************************/
				case UART_RX_HEART_BEAT_ACK_RECEIVED:
				{
					last_uart_command_ack_pending = false;
				}
				break;
/*********************************************************************************************************/
				case UART_RX_CAN_ACK_RECEIVED:
				{
					last_can_command_ack_pending = false;
				}
				break;
/********************************************************************************************************/
				default:
				{

				}
				break;
/********************************************************************************************************/

			}
		}

	}
}





