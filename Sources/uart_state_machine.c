/*
 * uart_state_machine.c
 *
 *  Created on: 15-Jul-2022
 *      Author: bhaar
 */

#include "uart_queue.h"
#include "uart.h"
#include "operating_system.h"
#include "rr_nvic.h"
#include "uart_state_machine.h"



volatile U16 new_rx_count_length_16 = 1;


UART_data_struct uart_rx_data;

U8 heart_beat_flag = 0;
U8 last_uart_command_ack_pending = false;
U8 bootloader_enable_command_received_u8 = false;


U8 heart_beat_check(void)
{
#if 1
	static uint32_t time_to_get_ack_old =0;
	static uint32_t time_to_get_ack_new = 0;
	uint32_t time_taken = 0;
	static uint32_t heart_beat_miss_count = 0;
	if(time_to_get_ack_old == 0)
	{
		OSIF_TimeDelay(0);
		time_to_get_ack_old = OSIF_GetMilliseconds();
	}
	//OSIF_TimeDelay(0);
	time_to_get_ack_new = OSIF_GetMilliseconds();
	time_taken = time_to_get_ack_new - time_to_get_ack_old;
#endif
	//static uint32_t time_taken = 0;
	//static uint32_t heart_beat_miss_count = 0;
	//time_taken++;
	if(time_taken > 5000)
	{
		time_taken = 0;
		time_to_get_ack_old = 0;
		if(0 == heart_beat_flag)
		{
			heart_beat_miss_count++;
			//if count greater than x do something
			if(heart_beat_miss_count > 5 && heart_beat_miss_count < 10)
			{
				//reinitialize uart
			}
			else if(heart_beat_miss_count > 10)
			{
				//reset N58 or self
			}
		}
		else
		{
			heart_beat_miss_count = 0;
		}
		send_heartbeat();
		//count heartbeat missed flag
	}
	return 0;
}



U8 send_ack_nack(U8 status, U8 command, U8 *payload, U16 length)
{
    U8 can_data[100] = {0,};

    can_data[0] = 0xDE;
    can_data[1] = 0xAD;
    can_data[2] = command;
    can_data[3] = (length+1) & 0xFF;		//length lo byte //status length is one more byte
    can_data[4] = (length+1) >> 8;		//length hi byte
    can_data[5] = status;
    memcpy(&can_data[6], payload, (length)); //0 to 5 header 6+ length, payload
    can_data[length + 7] = 0x00;
    can_data[length + 8] = 0x00;
    can_data[length + 9 ] = 0x03;

    uart_tx_data((uint8_t *)can_data, (length+9));

    return 0;

}


U8 send_heartbeat(void)
{
    U8 can_data[100] = {0,};
    U16 length = 0;

    can_data[0] = 0xDE;
    can_data[1] = 0xAD;
    can_data[2] = 0x00;
    can_data[3] = (length+1) & 0xFF;		//length lo byte //status length is one more byte
    can_data[4] = (length+1) >> 8;		//length hi byte
    can_data[5] = 0;
    can_data[length + 6] = 0xDE;
    can_data[length + 7] = 0xAD;
    can_data[length + 8] = 0x03;

    uart_tx_data(can_data, (length+9));
    last_uart_command_ack_pending = true;

    return 0;

}

void execute_command(UART_data_struct uart_strcut)
{
	operating_system_uart_rx_queue_tst uart_received_ack_st;
	switch (uart_strcut.cmd_u8)
	{
/***********************************************************************************************/
		case UART_HEART_BIT_ACK_COMMAND:
		{
			if(uart_strcut.payload_au8[0] == 0x00)
			{
				heart_beat_flag = 1;
				uart_received_ack_st.event_e = UART_RX_HEART_BEAT_ACK_RECEIVED;
				uart_received_ack_st.source_u8 = uart_strcut.cmd_u8;
				xQueueSendFromISR(os_uart_rx_queue_handler_ge,&uart_received_ack_st,NULL);
			}
			else
			{
				heart_beat_flag = 0;
			}
		}
		break;
/***********************************************************************************************/
	    case UART_CAN_ACK_COMMAND:
	    {
	        	last_can_command_ack_pending = false;
	        	uart_received_ack_st.event_e = UART_RX_CAN_ACK_RECEIVED;
	        	uart_received_ack_st.source_u8 = uart_strcut.cmd_u8;
	        	xQueueSendFromISR(os_uart_rx_queue_handler_ge,&uart_received_ack_st,NULL);
	     }
	     break;
/***********************************************************************************************/
	    case UART_BOOTLOADER_ENABLE_COMMAND:
	    {
	    		bootloader_enable_command_received_u8 = true;
	    		uart_receive_counter_u16 = 1;
	    		uart_received_ack_st.event_e = UART_BOOTLOADER_CMD_RECEIVED;
	    		uart_received_ack_st.source_u8 = uart_strcut.cmd_u8;
	    	    xQueueSendFromISR(os_uart_rx_queue_handler_ge,&uart_received_ack_st,NULL);

	    }
	    break;
/***********************************************************************************************/
	    default:
	    {

	    }
	    break;
/************************************************************************************************/

	}

}
U8 process_uart_data(U8 data)
{
	static uint8_t data_process_state = STX1_STATE;
	static UART_data_struct uart_rx_data;
	static uint8_t len_byte1;
	static uint8_t len_byte2;
	static uint8_t crc_byte_1;
	static uint8_t crc_byte_2;
	static uint16_t length_16;
	static uint16_t crc_16;
	static uint16_t len_counter = 0;

	switch (data_process_state)
	{
/**************************************************************************************************/
		case STX1_STATE:
		{
			if (data == 0xDE)
			{
				data_process_state = STX2_STATE;
			}
		}
		break;
/**************************************************************************************************/
		case STX2_STATE:
		{
			if (data == 0xAD)
			{
				data_process_state = CMD_STATE;
				uart_rx_data.stx_U16 = (U16) (0xDE << 8 | 0xAD);
			}
			else
			{
				data_process_state = STX1_STATE;
			}
		}
		break;
/**************************************************************************************************/
		case CMD_STATE:
		{
			data_process_state = LEN1_STATE;
			uart_rx_data.cmd_u8 = data;
		}
		break;
/**************************************************************************************************/
		case LEN1_STATE:
		{
			len_byte1 = data;
			data_process_state = LEN2_STATE;
		}
		break;
/**************************************************************************************************/
		case LEN2_STATE:
		{
			len_byte2 = data;
			length_16 = (U16) (len_byte2 << 8 | len_byte1);
			uart_rx_data.length_u16 = length_16;
			data_process_state = DATA_STATE;
			if (length_16 > 4096 || length_16 <= 0)
			{
				data_process_state = STX1_STATE;
			}
		}
		break;
/**************************************************************************************************/
		case DATA_STATE:
		{
			len_counter++;
			uart_rx_data.payload_au8[len_counter - 1] = data;
			if (length_16 > len_counter)
			{
				data_process_state = DATA_STATE;
			}
			else
			{
				data_process_state = CRC1_STATE;
				len_counter = 0;
			}
		}
		break;
/**************************************************************************************************/
		case CRC1_STATE:
		{
			crc_byte_1 = data;
			data_process_state = CRC2_STATE;
		}
		break;
/**************************************************************************************************/
		case CRC2_STATE:
		{
			crc_byte_2 = data;
			data_process_state = ETX_STATE;
			crc_16 = (U16) (crc_byte_2 << 8 | crc_byte_1);
			uart_rx_data.crc_u16 = crc_16;
		}
		break;
/**************************************************************************************************/
		case ETX_STATE:
		{
			if (data == 0x03)
			{
				execute_command(uart_rx_data);
				data_process_state = STX1_STATE;
			}
		}
		break;
/**************************************************************************************************/
		default:
		{
			data_process_state = STX1_STATE;
		}
		break;
	}
	return 0;
}







