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
#include "uart_tx_task.h"
#include "tork_update_app.h"

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
						if( deac_heart_beat != 1)                      //added new condition
 						{
							send_heartbeat();                          //uncommented
							heart_beat_mst_try_u8 = 0;
						}
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
							if(last_uart_command_ack_pending == true && (deac_heart_beat!=1))       //added additional condition
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
						if(deac_heart_beat != 1)                      //added additional condition
						{
							last_can_command_ack_pending = true;
							uart_send_received_can_data();
							can_mst_try_u8 = 0;
						}
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
							if(last_can_command_ack_pending && (deac_heart_beat!=1))       //added additional condition
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
				case TORK_BLDR_INIT_ACK_EVT:
				{
					tork_update_app_uart_cmd_send_v(0x05,0xAA);
				}
				break;
/************************************************************************************************************/
				case TORK_BLDR_CODE_SIZE_CMD_ACK_EVT:
				{
					tork_update_app_uart_cmd_send_v(0x06,0xAA);
				}
				break;
/************************************************************************************************************/
				case TORK_BLDR_ERASE_CMD_ACK_EVT:
				{
					tork_update_app_uart_cmd_send_v(0x07,0xAA);
				}
				break;
/************************************************************************************************************/
				case TORK_BLDR_WRITE_DATA_BLOCK_ACK_EVT:
				{
					tork_update_app_uart_cmd_send_v(0x08,0xAA);
				}
				break;
/************************************************************************************************************/
				case TORK_BLDR_JMP_CMD_ACK_EVT:
				{
					tork_update_app_uart_cmd_send_v(0x09,0xAA);
				}
				break;
/************************************************************************************************************/
				case TORK_BLE_DATA_FRAME_EVT:
				{
					operating_system_can_rx_msg_queue_tst can_rx_msg_st;
					if(xQueueReceive(os_tork_ble_data_queue_handler_ge, &can_rx_msg_st, portMAX_DELAY))
					{
//						uint32_t can_msg_id = 0x101;
//						uint8_t can_buff[8] = {0x01, 0x02, 0x3, 0x4, 0xAA, 0xBB, 0xCC, 0xDD};
						uart_send_single_can_msg(0xBB, can_rx_msg_st.received_can_msg.id, (uint8_t *)(can_rx_msg_st.received_can_msg.data));
//						uart_send_single_can_msg(0xBB, can_msg_id, can_buff);
					}
				}
				break;
				default:
				{

				}
				break;
/*************************************************************************************************************/
			}

		}
	}
}
