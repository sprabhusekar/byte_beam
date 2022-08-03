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
	while(1)
	{
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
