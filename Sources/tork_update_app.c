
#include "tork_update_app.h"
#include "operating_system.h"
#include "can_tx_task.h"
#include "uart.h"
#include "can.h"
//#include "SEGGER_RTT.h"

uint8_t deac_heart_beat = 0;

int tork_update_app_process_can_msg(can_message_t message_can)
{
	static tork_bldr_state_te tork_bldr_state_e = TORK_BLDR_INIT_ACK;
	operating_system_uart_tx_queue_tst uart_tx_que_st;

	switch(tork_bldr_state_e)
	{
		case TORK_BLDR_INIT_ACK:
		{
			if(message_can.id == 0x10F)
			{
				deac_heart_beat = 1;
				tork_bldr_state_e = TORK_BLDR_CODE_SIZE_CMD_ACK;
				uart_tx_que_st.event_e = TORK_BLDR_INIT_ACK_EVT;
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
			}
		}
		break;
		case TORK_BLDR_CODE_SIZE_CMD_ACK:
		{
			if(message_can.id == 0x10F)
			{
				tork_bldr_state_e = TORK_BLDR_ERASE_CMD_ACK;
				uart_tx_que_st.event_e = TORK_BLDR_CODE_SIZE_CMD_ACK_EVT;
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
			}
		}
		break;
		case TORK_BLDR_ERASE_CMD_ACK:
		{
			if(message_can.id == 0x10F)
			{
				tork_bldr_state_e = TORK_BLDR_WRITE_DATA_BLOCK_ACK;
				uart_tx_que_st.event_e = TORK_BLDR_ERASE_CMD_ACK_EVT;
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
			}
		}
		break;
		case TORK_BLDR_WRITE_DATA_BLOCK_ACK:
		{
			if(message_can.id == 0x10F && jmp_cmd_sent_u8 != 1)
			{
				//SEGGER_RTT_printf(0,"ack val is %x \r\n",message_can.data[1]);
				uart_tx_que_st.event_e = TORK_BLDR_WRITE_DATA_BLOCK_ACK_EVT;
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
			}
			if(message_can.id == 0x10F && jmp_cmd_sent_u8 == 1)
			{
				uart_tx_que_st.event_e = TORK_BLDR_JMP_CMD_ACK_EVT;
				BaseType_t xHigherPriorityTaskWoken = pdFALSE;
				xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
				jmp_cmd_sent_u8 = 0;
				tork_bldr_state_e = TORK_BLDR_INIT_ACK;
				deac_heart_beat = 0;
			}
		}
		break;
		default:
		{

		}
		break;
	}

	return 0;
}


void tork_update_app_uart_cmd_send_v(U8 cmd,U8 ack_status)
{

   U8 can_data_buff[10] = {0,};
   U16 length = 1;
   can_data_buff[0] = 0xDE;
   can_data_buff[1] = 0xAD;
   can_data_buff[2] = cmd;
   can_data_buff[3] = length & 0xFF;    //length lo byte //status length is one more byte
   can_data_buff[4] = length >> 8;		//length hi byte
   can_data_buff[5] = ack_status;
   can_data_buff[6] = 0xDE;
   can_data_buff[7] = 0xAD;   //TODO::implement CRC check
   can_data_buff[8] = 0x03;
   uart_tx_data(can_data_buff, 9);
}
