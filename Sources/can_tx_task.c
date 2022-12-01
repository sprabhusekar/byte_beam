/*
 * can_tx_task.c
 *
 *  Created on: 23-11-22
 *      Author: Pradeep
 */

#include "operating_system.h"
#include "can.h"
#include "can_tx_task.h"

operating_system_can_tx_queue_tst can_tx_queue_gst;
uint8_t jmp_cmd_sent_u8 = 0;

void can_comm_tx_task_v(void *parameter)
{
	while(1)
	{
		if(xQueueReceive(os_can_tx_queue_handler_ge, &can_tx_queue_gst, portMAX_DELAY))
		{
			can_message_t can_msg;
			can_msg.id = (((uint32_t)(can_tx_queue_gst.can_data_struct.can_payload[3])) << 24) |
					 	 (((uint32_t)(can_tx_queue_gst.can_data_struct.can_payload[2])) << 16) |
						 (((uint32_t)(can_tx_queue_gst.can_data_struct.can_payload[1])) << 8)  |
						 ((uint32_t)(can_tx_queue_gst.can_data_struct.can_payload[0]));
			if((can_msg.id & 0x0000FFFF) == 0xF08F)
			{
				jmp_cmd_sent_u8 = 1;
			}
			can_msg.length = 8;
			int loop_var = 0;
			for(loop_var = 0;loop_var < 8;loop_var++)
			{
				can_msg.data[loop_var] = can_tx_queue_gst.can_data_struct.can_payload[loop_var+4];
			}
			CAN_Send(&can_pal1_instance, CAN_TX_MAILBOX0, &can_msg);
		}
	}
}
