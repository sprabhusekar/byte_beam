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
#include "bootloader_app.h"
operating_system_uart_rx_queue_tst queue_received_uart_ack_gst;

void uart_rx_task_v(void *parameter)
{
	(void)(parameter);
	operating_system_uart_tx_queue_tst uart_rx_tx_que_st;

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
				case UART_BOOTLOADER_CMD_RECEIVED:
				{
					bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
					//received_boot_config_st.new_firmware_status_u8 = 0xCC;
					//uart_rx_tx_que_st.event_e =  UART_BOOTLOADER_ENABLE_CMD_ACK_SEND;
					//uart_rx_tx_que_st.source_u8 = last_can_command_ack_pending;
				   //xQueueSend(os_uart_tx_queue_handler_ge,&uart_rx_tx_que_st,100);

				}
				break;
/*********************************************************************************************************/
				case UART_BOOT_CONFIG_CMD_RECEIVED:
				{
					//bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
					//bootloader_enable_cmd_procees();
				}
				break;
/*********************************************************************************************************/
				case UART_BOOTLOADER_FIRMWARE_DATA_RECEIVED:
				{
					//bootloader_new_firmware_receive();
				}
				break;
/*********************************************************************************************************/
				case UART_BOOTLOADER_END_OF_DATA_RECEIVED:
				{
					//bootloader_end_of_data_handling();
				}
				break;
/*********************************************************************************************************/
				case UART_BOOTLOADER_WHOLE_CRC_CHECK_CMD_RECEIVED:
				{
					//bootloader_whole_crc_check();
				}
				break;
/**********************************************************************************************************/
				case UART_BOOTLOADER_JUMPT_TO_BOOTLOADER_SECTION_CMD_RECEIVED:
				{
					//bootloader_jumping_to_boot_section();
				}
				break;
/**********************************************************************************************************/
				default:
				{

				}
				break;
/********************************************************************************************************/

			}
		}

	}
}





