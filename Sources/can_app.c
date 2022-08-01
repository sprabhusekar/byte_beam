/*
 * can_app.c
 *
 *  Created on: 19-Jul-2022
 *      Author: bhaar
 */
#include "can_pal.h"
#include "can_queue.h"
#include "can.h"
#include "uart_state_machine.h"
#include "uart.h"
#include "operating_system.h"
#include "can_app.h"
#if 1
U8 can_data[14 * CAN_QUEUE_SIZE + 8] = { 0x02, 0x04 };


volatile U8 can_queue_timeout_vu8 = 0;

volatile U8 can_queue_timeout_check_vu8 = 0;
 U16 can_queue_check_counter_16 = 0;

 U16 total_uart_length_u16  = 0;

 operating_system_uart_tx_queue_tst can_uart_tx_queue_data_st;

void can_data_push(void)
{
	can_message_t rx_can_data_read;
	static U32 i = 0;
	U16 length = 0;
	can_data[0] = 0xDE;
	can_data[1] = 0xAD;
	can_data[2] = 0x04;
	U16  can_queue_size_u16 = can_rx_array_queue_size();
   if(can_rx_array_queue_size())
   {
	for (i = 0; i < can_queue_size_u16; i++)
	{
		if (can_rx_array_queue_get(&rx_can_data_read) == CAN_QUEUE_SUCCESS) {
			can_data[5 + i * 14] = 0x00;		//CAN type for extended ID
			can_data[6 + i * 14] = rx_can_data_read.length;		//DLC
			can_data[7 + i * 14] = rx_can_data_read.id >> 24;
			can_data[8 + i * 14] = rx_can_data_read.id >> 16;
			can_data[9 + i * 14] = rx_can_data_read.id >> 8;
			can_data[10 + i * 14] = rx_can_data_read.id;
			can_data[11 + i * 14] = rx_can_data_read.data[0];
			can_data[12 + i * 14] = rx_can_data_read.data[1];
			can_data[13 + i * 14] = rx_can_data_read.data[2];
			can_data[14 + i * 14] = rx_can_data_read.data[3];
			can_data[15 + i * 14] = rx_can_data_read.data[4];
			can_data[16 + i * 14] = rx_can_data_read.data[5];
			can_data[17 + i * 14] = rx_can_data_read.data[6];
			can_data[18 + i * 14] = rx_can_data_read.data[7];



			//add length here

		}
		else {
			i--;


		}
	}

	length = i * 14;
	can_data[3] = length & 0xFF;		//length lo byte
	can_data[4] = length >> 8;		//length hi byte
	can_data[5 + i * 14] = 0xAA;
	can_data[6 + i * 14] = 0xBB;
	can_data[7 + i * 14] = 0x03;
	if(length != 0)
	{
		total_uart_length_u16 = length+8;
		can_uart_tx_queue_data_st.event_e =  UART_TX_CAN_DATA_SEND;
		can_uart_tx_queue_data_st.source_u8 =  0xAA;
		xQueueSend(os_uart_tx_queue_handler_ge,&can_uart_tx_queue_data_st,1);
	}
 }

}
#endif

void uart_send_received_can_data(void)
{
	uart_tx_data(can_data, total_uart_length_u16);
	last_can_command_ack_pending = true;
}



