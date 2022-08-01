/*
 * timer.h
 *
 *  Created on: 11-Jun-2022
 *      Author: bhaar
 */

#ifndef TIMER_H_
#define TIMER_H_

extern volatile uint8_t timer_pal_chan0_100ms_timer_call_back_vu8;
extern volatile uint8_t timer_pal_chan2_10ms_timer_call_back_vu8;
extern volatile uint8_t timer_pal_chan4_5ms_timer_call_back_vu8;

uint8_t timer_init_u8(void);
void timer_start_base_v(void);
extern uint32_t timeout_uart_ack_flag_counter_u32;

#endif /* TIMER_H_ */
