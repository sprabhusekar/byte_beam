/*
 * app_main.c
 *
 *  Created on: 22-Jul-2022
 *      Author: bhaar
 */


#include "core_main.h"
#include "uart_tx_task.h"
#include "internal_flash_app.h"
//#include "SEGGER_RTT.h"

void app_main(void)
{
	U8 app_run_sts_u8 = 0;
	app_run_sts_u8 =  core_main_u8();
//	SEGGER_RTT_printf(0,"Core Initiated\r\n");
	app_run_sts_u8 += rtos_init_u8();
//	SEGGER_RTT_printf(0,"RTOS Initiated\r\n");
	if(0 == app_run_sts_u8)
	{
	   rtos_run();
	}
	else
	{

	}
}

