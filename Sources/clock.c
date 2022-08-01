/*
 * clock.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */
#include "clock.h"

#include "clockMan1.h"


uint8_t clock_init_u8(void)
{
	uint8_t clock_init_sts_u8 = 0;
	clock_init_sts_u8 = CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
	    				   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	clock_init_sts_u8 +=  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
	clock_init_sts_u8 += POWER_SYS_Init(powerConfigsArr, POWER_MANAGER_CONFIG_CNT, &pwrMan1_InitConfig0, 0);
	return clock_init_sts_u8;
}
