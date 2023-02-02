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
#include "SEGGER_RTT.h"
#include "pin_mux.h"

uint8_t ignition_status = 0;
uint32_t io_port_status = 0;

extern volatile int test_counter;
void diagnostic_task_v(void *parameter)
{
	(void)(parameter);
	uart_rx_data_queue();
	timer_start_base_v();
	nvic_enable_all_Interrupts_v();
	int temp_counter = 0;
	while(1)
	{
		io_port_status = PINS_DRV_ReadPins(PTE);
		if((io_port_status & (uint32_t)(1<<5)) == 32)
		{
			ignition_status = 0;
		}
		else
		{
			ignition_status = 1;
		}
		uart_rx_data_queue();
		vTaskDelay(1);
	}
}
