/*
 * timer.c
 *
 *  Created on: 11-Jun-2022
 *      Author: bhaar
 */

#include "s32k144.h"
#include "Cpu.h"
#include "common_header.h"
#include "uart_state_machine.h"
#include "can_app.h"
#include "operating_system.h"
#include "timer.h"
#include "tork_update_app.h"

#define N58_COMMUNICATION_TIMEOUT   20

volatile U8 timer_pal_chan0_2ms_timer_call_back_vu8 = 0;

volatile U8 timer_pal_chan2_10ms_timer_call_back_vu8 = 0;

volatile U8 timer_pal_chan4_5ms_timer_call_back_vu8 = 0;

uint32_t timeout_uart_ack_flag_counter_u32 = 0;

volatile U8 n58_communication_start_vu8 = 0;

uint32_t ftm_tim_sys_freq_u32[FTM_INSTANCE_COUNT];
uint8_t  timer_init_u8(void)
{
	uint8_t return_value_u8 = 0;
    if(TIMING_Init(&timing_pal1_instance, &timing_pal1_InitConfig))
    {
    	return_value_u8= 1;
    }
   ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx] = FTM_DRV_GetFrequency(timing_pal1_instance.instIdx);
   return return_value_u8;
}

void timer_start_base_v(void)
{
	TIMING_StartChannel(&timing_pal1_instance,0, (30* ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx])/1000);     //changed from 50 to 30 because uart thread is running at 50
	TIMING_StartChannel(&timing_pal1_instance,1, (1000* ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx])/1000);
	//TIMING_StartChannel(&timing_pal1_instance,2, (10* ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx])/1000);
	//TIMING_StartChannel(&timing_pal1_instance,3, (1000* ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx])/1000);
//	TIMING_StartChannel(&timing_pal1_instance,4, (5* ftm_tim_sys_freq_u32[timing_pal1_instance.instIdx])/1000);

}

void timer_pal1_chan0_callback_100ms_v(void *userdata)
{
	timer_pal_chan0_2ms_timer_call_back_vu8 = 1;
}


void timer_pal_chan1_callback_50ms_v(void *userdata)
{

}

void timer_pal_chan0_callback_1000ms_v(void *userdata)
{
	(void)(userdata);
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(can_rx_intrupt_Semaphore,&xHigherPriorityTaskWoken);
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void timer_pal_chan1_callback_5000ms_v(void *userdata)
{
	static uint8_t timer_5s_counter_u8 = 0;
	static uint8_t timer_10s_counter_u8 = 0;
	static uint8_t timer_150s_counter_u8 = 0;
	static uint8_t ignition_timer_u8 = 0;
//	static uint8_t test_count_u8 = 0;
	U32 dummy_byte_u32 = 100;
	operating_system_uart_tx_queue_tst uart_tx_que_st;

	static uint8_t boot_toggle = 0;
	if(boot_toggle == 0)
	{
		PINS_DRV_WritePin(PTA, 11, 0);
	}
	else if(boot_toggle == 1)
	{
		PINS_DRV_WritePin(PTA, 11, 1);
	}
	else if(boot_toggle ==2)
	{
		boot_toggle =100;
	}

	if(boot_toggle <3)
	{
		boot_toggle++;
	}

	if(0 == n58_communication_start_vu8)
	{
		timer_10s_counter_u8++;
		if(timer_10s_counter_u8 >= N58_COMMUNICATION_TIMEOUT)
		{
			timer_10s_counter_u8 = N58_COMMUNICATION_TIMEOUT + 2;
			n58_communication_start_vu8 = 1;
		}
	}

	if(n58_communication_start_vu8)
	{
		timer_5s_counter_u8++;
		ignition_timer_u8++;
		if(timer_5s_counter_u8 >=5)
		{
			timer_5s_counter_u8  = 0;
			uart_tx_que_st.event_e =  UART_TX_HEART_BEAT_SEND;
			uart_tx_que_st.source_u8 = dummy_byte_u32;
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		//xSemaphoreGiveFromISR(uart_tx_intrupt_Semaphore,&xHigherPriorityTaskWoken);
			xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);         //uncommented
//			if(test_count_u8 < 5)
//			{
//				test_count_u8++;
//			}
//			if(test_count_u8 >= 5 && test_count_u8 <30)
//			{
//				if(test_count_u8 == 6)
//				{
//					PINS_DRV_WritePin(PTA, 12, 1);
//				}
//				if(test_count_u8 == 25)
//				{
//					PINS_DRV_WritePin(PTA, 12, 0);
//				}
//				test_count_u8++;
//			}
		}
		if(ignition_timer_u8 >=10)
		{
			ignition_timer_u8 = 0;
			uart_tx_que_st.event_e = IGNITION_STATUS_EVT;
			BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			xQueueSendFromISR(os_uart_tx_queue_handler_ge,&uart_tx_que_st,&xHigherPriorityTaskWoken);
		}

		timer_150s_counter_u8++;

		if (timer_150s_counter_u8 >= 150)
		{
			timer_150s_counter_u8 = 0;
			if(deac_heart_beat != 1)
			{
				if(heart_beat_received == 1)
				{
					heart_beat_received = 0;
				}
				else
				{
//					SystemSoftwareReset();
					boot_toggle = 0;

				}
			}
		}

	}
}

void timer_pal_chan3_callback_1s_v(void *userdata)
{

}

void timer_pal_cha4_callback_5ms_v(void *userdata)
{
	timer_pal_chan4_5ms_timer_call_back_vu8 = 1;
}
