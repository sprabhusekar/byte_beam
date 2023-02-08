/*
 * can_rx_task.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */

#include "operating_system.h"
#include "common_header.h"
#include "uart_state_machine.h"
#include "can.h"
#include "can_app.h"

void can_comm_rx_task_v(void *parameter)
{
	(void)(parameter);
	//OSIF_TimeDelay(20000);
	while(1)
	{
//		can_message_t can_simulated_pkt;
//		memset(&can_simulated_pkt, 0, sizeof(can_simulated_pkt));
//		can_simulated_pkt.id = 0x300;
//		can_simulated_pkt.data[0] = 1;
//		can_simulated_pkt.length = 8;
//		can_rx_array_queue_put(can_simulated_pkt);
		xSemaphoreTake(can_rx_intrupt_Semaphore, portMAX_DELAY);
		if((last_uart_command_ack_pending == false)
									&& (last_can_command_ack_pending == false))
		{
			can_data_push();
		}
		else
		{

		}
	}
}
