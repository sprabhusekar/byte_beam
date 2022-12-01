/*
 * uart.c
 *
 *  Created on: 11-Jun-2022
 *      Author: bhaar
 */

#include "uart_pal1.h"
#include "uart_queue.h"
#include "uart_state_machine.h"

U8 uart_data = 0;

U8 uart_received_data_au8[525]; // 512 bytes + 9 =

U16 uart_receive_counter_u16 = 21;


U32 uart_rx_data_queue(void)
{
	//return UART_ReceiveData(&uart_pal1_instance, uart_received_data_au8, new_rx_count_length_16);
	return UART_ReceiveData(&uart_pal1_instance, uart_received_data_au8, uart_receive_counter_u16);
	//UART_ReceiveDataBlocking(&uart_pal1_instance, uart_received_data_au8, 1, 1);
	//while(UART_GetReceiveStatus(&uart_pal1_instance, &bytesRemaining) == STATUS_BUSY);
}

U8 uart_init(void)
{
	U8 uart_init_sts_u8 = 0;

    if(UART_Init(&uart_pal1_instance, &uart_pal1_Config0))
    {
    	uart_init_sts_u8 = 1;
    }
    uart_rx_data_queue();

    return uart_init_sts_u8;
}

U8 uart_tx_data(U8 *tx_buffer_au8,U32 tx_size_u32)
{
	 uint8_t uart_tx_sts_u8 = 0;
	 //if(UART_SendData(&uart_pal1_instance, tx_buffer_au8,tx_size_u32))
	 if(UART_SendDataBlocking(&uart_pal1_instance,tx_buffer_au8,tx_size_u32,10))
	 {
		 uart_tx_sts_u8 = 1;
	 }
	 return uart_tx_sts_u8;
}

U8 uart_rx_data_u8 = 0 ;

void uart0_rx_callback(void *driverState, uart_event_t event, void *userData)
{
	COM_HDR_UNUSED(driverState);
	COM_HDR_UNUSED(userData);
	U8  rx_counter_u8 = 0;
    switch(event)
    {
/***************************************************************************************************/
       case  UART_EVENT_RX_FULL:
   	   {
   		   if(0 == bootloader_enable_command_received_u8)
   		   {
//   			   for(rx_counter_u8 = 0;rx_counter_u8 <9;rx_counter_u8++)
//   			   {
//   				   process_uart_data(uart_received_data_au8[rx_counter_u8]);
//   			   }
//   			   if(uart_rx_data_queue())
//   			   {
//   				   uart_rx_data_queue();
//   			   }
   			   for(rx_counter_u8 = 0;rx_counter_u8 <21;rx_counter_u8++)
			   {
				   process_uart_data(uart_received_data_au8[rx_counter_u8]);
			   }
			   if(uart_rx_data_queue())
			   {
				   uart_rx_data_queue();
			   }
   		   }
   		   else
   		   {
   			   for(rx_counter_u8 = 0;rx_counter_u8 <1;rx_counter_u8++)
   			   {
   				   process_uart_data(uart_received_data_au8[rx_counter_u8]);
   			   }
   		   }
//		   for(rx_counter_u8 = 0;rx_counter_u8 <1;rx_counter_u8++)
//		   {
//			   process_uart_data(uart_received_data_au8[rx_counter_u8]);
//		   }

   	   }
   	   break;
 /*****************************************************************************************************/
   	   case UART_EVENT_ERROR:
   	   {
   		   //UART_Deinit(&uart_pal1_instance);
   		   //uart_init();
   	   }
   	   break;
 /*****************************************************************************************************/
   	   case UART_EVENT_TX_EMPTY:
   	   {
   		   //uart_rx_array_queue_put(uart_data);
   	   }
   	   break;
 /******************************************************************************************************/
   	   case UART_EVENT_END_TRANSFER:
   	   {
   		   //uart_rx_array_queue_put(uart_data);
   	   }
   	   break;
/**********************************************************************************************************/
   	   default:
   	   {

   	   }
   	   break;
/*********************************************************************************************************/
   }
}


