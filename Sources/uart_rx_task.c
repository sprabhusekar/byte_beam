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
#include "internal_flash_app.h"
#include "uart_tx_task.h"
#include "uart_rx_task.h"
#include "bb_memory.h"
#include "uart.h"


bootloader_config_tst received_boot_config_st;
bootloader_config_tst flash_read_boot_config_st;
bootloader_config_tst boot_loader_conf_gst;






operating_system_uart_rx_queue_tst queue_received_uart_ack_gst;
static void bootloader_jumping_to_boot_section(void);

//static void bootloader_app_uart_cmd_ack_send_v(U8 cmd,U8 cmd_ack_sts);

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
					bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
					//bootloader_enable_cmd_procees();
					 //bootloader_jumping_to_boot_section();
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



static void bootloader_jumping_to_boot_section(void)
{

	U8 write_data_au8[16];
	U32 int_flash_write_data_u32 = 0;
	if(in_flash_app_erase_e(BOOTLOADER_CONFIG_START_ADDRESS, BOOTLOADER_CONFIG_END_ADDRESS) == IN_FLASH_SUCCESS)
	{
		memory_copy_u8_array_v((U8*)write_data_au8, (U8*)&received_boot_config_st, sizeof(received_boot_config_st));
		memory_copy_u8_array_v((U8*)&boot_loader_conf_gst,  (U8*)&received_boot_config_st, sizeof(received_boot_config_st));
		int_flash_write_data_u32 = (U32)&write_data_au8;
		if(internal_flash_app_write_e(BOOTLOADER_CONFIG_START_ADDRESS, write_data_au8, 16) == IN_FLASH_SUCCESS)
		{
			if(internal_flash_app_read_e(BOOTLOADER_CONFIG_START_ADDRESS,
											(U8*)&flash_read_boot_config_st, sizeof(received_boot_config_st)) == IN_FLASH_SUCCESS)
			{
				if(!memory_comp_u8((U8*)&received_boot_config_st,
	        					  (U8*)&flash_read_boot_config_st,  sizeof(received_boot_config_st)))
				{
					//bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID,
						//							  BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS,
							//	 					  BAPP_CAN_SUCCESS_STS);
					//boot_loader_app_jumping_to_Bootloader_section_e();
					SystemSoftwareReset();
				}
			}
			else
			{
				bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
			}
		}
		else
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
		}
	}
	else
	{
		bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
	}

}


/**
=====================================================================================================================================================

@fn Name            : bootloader_app_uart_cmd_ack_send_v
@b Scope            : Private
@n@n@b Description  : Sends Bootloader Cmd through CAN0
@param Input Data   : command_aru8, command_status_aru8
@return Return Value: NULL

=====================================================================================================================================================
*/
 void bootloader_app_uart_cmd_ack_send_v(U8 cmd,U8 cmd_ack_sts)
{

    U8 can_data[100] = {0,};
    U16 length = 0;
    can_data[0] = 0xDE;
    can_data[1] = 0xAD;
    can_data[2] = cmd;
    can_data[3] = (length+1) & 0xFF;		//length lo byte //status length is one more byte
    can_data[4] = (length+1) >> 8;		//length hi byte
    can_data[5] = cmd_ack_sts;
    can_data[length + 6] = 0xDE;
    can_data[length + 7] = 0xAD;   //TODO::implement CRC check
    can_data[length + 8] = 0x03;
    uart_tx_data(can_data, (length+9));
}


