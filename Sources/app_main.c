/*
 * app_main.c
 *
 *  Created on: 22-Jul-2022
 *      Author: bhaar
 */


#include "core_main.h"

void app_main(void)
{
	U8 app_run_sts_u8 = 0;
	app_run_sts_u8 =  core_main_u8();
	app_run_sts_u8 += rtos_init_u8();
	if(0 == app_run_sts_u8)
	{
	   rtos_run();
	}
	else
	{

	}
}

