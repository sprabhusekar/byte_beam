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
#include "bootloader_app.h"

operating_system_uart_tx_queue_tst queue_received_uart_send_gst;

uint8_t can_mst_try_u8 = 0;
uint8_t heart_beat_mst_try_u8 = 0;
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
						heart_beat_mst_try_u8 = 0;
					}
					else
					{
						heart_beat_mst_try_u8++;
						if(heart_beat_mst_try_u8 >=5)
						{
							heart_beat_mst_try_u8 = 6;
						}
						else
						{
							if(last_uart_command_ack_pending == true)
								send_heartbeat();
							//else
								//uart_send_received_can_data();
						}

					}
				}
				break;
/***********************************************************************************************************/
				case UART_TX_CAN_DATA_SEND:
				{
					if((last_uart_command_ack_pending == false)
						&& (last_can_command_ack_pending == false))
					{
						last_can_command_ack_pending = true;
						uart_send_received_can_data();
						can_mst_try_u8 = 0;
					}
					else
					{
						can_mst_try_u8++;
						if(can_mst_try_u8 >= 5)
						{
							can_mst_try_u8 = 6;
						}
						else
						{
							if(last_can_command_ack_pending)
								uart_send_received_can_data();
							//else
								//send_heartbeat();
						}
					}
				}
				break;
/************************************************************************************************************/
				case UART_BOOTLOADER_ENABLE_CMD_ACK_SEND:
				{
					bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
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
