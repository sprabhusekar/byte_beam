/*
 * core_main.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */


#include "core_main.h"
#include "clock.h"
#include "gpio.h"
#include "uart.h"
#include "can.h"
#include "timer.h"
#include "operating_system.h"

uint8_t core_main_u8(void)
{
	uint8_t core_sts_u8 = 0;

	core_sts_u8 = clock_init_u8();
	core_sts_u8 += gpio_init_u8();
    can_init();
    core_sts_u8 += timer_init_u8();
    core_sts_u8 += uart_init();
    core_sts_u8 += nvic_init_v();
    nvic_disable_all_Interrupts_v();
    return core_sts_u8;
}

uint8_t rtos_init_u8(void)
{
	uint8_t rtos_sts_u8 = 0;
	rtos_sts_u8 = operating_system_init_u8();
   return rtos_sts_u8;
}

void rtos_run(void)
{
	 operating_system_start_scheduler_v();
}
