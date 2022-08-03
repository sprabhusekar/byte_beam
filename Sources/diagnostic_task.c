/*
 * diagnostic_task.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */
#include "diagnostic_task.h"
#include "timer.h"
#include "rr_nvic.h"
#include "uart.h"
#include "operating_system.h"



void diagnostic_task_v(void *parameter)
{
	(void)(parameter);
	uart_rx_data_queue();
	timer_start_base_v();
	nvic_enable_all_Interrupts_v();
	while(1)
	{
		uart_rx_data_queue();
		vTaskDelay(1);
	}
}
