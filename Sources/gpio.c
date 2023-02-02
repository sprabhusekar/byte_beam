/*
 * gpio.c
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */


#include "gpio.h"
#include "pin_mux.h"


uint8_t gpio_init_u8(void)
{
	uint8_t return_sts_u8 = 0;
	return_sts_u8 = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
	PINS_DRV_WritePin(PTD, 0, 0);
//	PINS_DRV_WritePin(PTA, 12, 0);
	return return_sts_u8;
}
