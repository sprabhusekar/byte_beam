/*
 * timer.h
 *
 *  Created on: 11-Jun-2022
 *      Author: bhaar
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "common_header.h"

extern volatile U8 timer_pal_chan0_100ms_timer_call_back_vu8;
extern volatile U8 timer_pal_chan2_10ms_timer_call_back_vu8;
extern volatile U8 timer_pal_chan4_5ms_timer_call_back_vu8;

U8 timer_init_u8(void);
void timer_start_base_v(void);
extern U32 timeout_uart_ack_flag_counter_u32;
extern volatile U8 n58_communication_start_vu8;

#endif /* TIMER_H_ */
