/*
 * operating_system.h
 *
 *  Created on: 04-Jul-2022
 *      Author: Prabhu
 */

#ifndef CONFIGURATION_LAYER_INC_OS_DRIVER_OPERATING_SYSTEM_H_
#define CONFIGURATION_LAYER_INC_OS_DRIVER_OPERATING_SYSTEM_H_

/* Includes for definitions */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "Cpu.h"
#include "common_header.h"
/* @{
 * Macro definition */
typedef enum operating_system_priority_te_tag
{
  osPriorityIdle          = 0,          // priority: idle (lowest)
  osPriorityLow           = 1,          // priority: low
  osPriorityBelowNormal   = 2,          // priority: below normal
  osPriorityNormal        = 3,          // priority: normal (default)
  osPriorityAboveNormal   = 4,          // priority: above normal
  osPriorityHigh          = 5,          // priority: high
  osPriorityRealtime      = 6,          // priority: realtime (highest)
  osPriorityError         = 0x84        // system cannot determine priority or thread has illegal priority
} operating_system_priority_te;


#define OS_UART_RX_TASK_PRIORITY		(osPriorityRealtime)
#define OS_UART_TX_TASK_PRIORITY		(osPriorityAboveNormal)
#define OS_CAN_RX_TASK_PRIORITY		 	(osPriorityHigh)
#define OS_CAN_TX_TASK_PRIORITY			(osPriorityAboveNormal)
#define OS_DIAGNOSTIC_TASK_PRIORITY		(osPriorityRealtime)


/* @} */

/* @{
 * Type definition */

/* @} */

/* @{
 *  Public Variable Definitions */
extern TaskHandle_t uart_rx_task_handle_ge;
extern TaskHandle_t uart_tx_task_handle_ge;

extern TaskHandle_t can_rx_task_handle_ge;

extern TaskHandle_t can_tx_task_handle_ge;

#define OS_UART_RX_QUEUE_SIZE        (5U)

typedef enum operating_system_uart_rx_queue_event_list_te_tag
{
	UART_RX_HEART_BEAT_ACK_RECEIVED,
	UART_RX_CAN_ACK_RECEIVED,
}operating_system_uart_rx_queue_event_list_te;

typedef struct operating_system_uart_rx_queue_tst_tag
{
	operating_system_uart_rx_queue_event_list_te event_e;
	uint8_t source_u8 ;
}operating_system_uart_rx_queue_tst;

typedef enum operating_system_uart_tx_queue_event_list_te_tag
{
	UART_TX_HEART_BEAT_SEND,
	UART_TX_CAN_DATA_SEND,
}operating_system_uart_tx_queue_event_list_te;

typedef struct operating_system_uart_tx_queue_tst_tag
{
	operating_system_uart_tx_queue_event_list_te event_e;
	uint8_t source_u8 ;
}operating_system_uart_tx_queue_tst;

extern QueueHandle_t os_uart_rx_queue_handler_ge;
extern QueueHandle_t os_uart_tx_queue_handler_ge;



extern SemaphoreHandle_t uart_rx_intrupt_Semaphore;

extern SemaphoreHandle_t uart_tx_intrupt_Semaphore;

extern SemaphoreHandle_t can_rx_intrupt_Semaphore;

extern SemaphoreHandle_t can_tx_intrupt_Semaphore;

/* @} */

/* Public Function Prototypes **/
extern uint8_t operating_system_init_u8(void);
extern void operating_system_start_scheduler_v(void);



#endif /* CONFIGURATION_LAYER_INC_OS_DRIVER_OPERATING_SYSTEM_H_ */
