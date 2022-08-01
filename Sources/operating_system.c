

/* Common Header Files */

#include "FreeRTOS.h"

#include "can_app.h"
#include "uart_state_machine.h"
#include "operating_system.h"
#include "uart_rx_task.h"
#include "uart_tx_task.h"
#include "diagnostic_task.h"
#include "can_rx_task.h"
#include "can_tx_task.h"
/* Driver Header File */


/** RTOS header **/
#include "task.h"
#include "queue.h"
#include "semphr.h"

/** @{
 * Private Definitions **/

/** @} */

/* Private Function Prototypes */

static uint8_t operating_system_create_semaphore_pru8(void);
static uint8_t operating_system_create_task_pru8(void);
static uint8_t operating_system_create_queue_pru8(void);

/** @{
 * Type definition */

/** @} */


/** @{
 * Private Variable Definitions */

/** @} */

/** @{
 *  Public Variable Definitions */




TaskHandle_t os_uart_rx_task_handle_ge = NULL;
TaskHandle_t os_uart_tx_task_handle_ge = NULL;

TaskHandle_t os_can_rx_task_handle_ge = NULL;

TaskHandle_t os_can_tx_task_handle_ge = NULL;

TaskHandle_t os_diagnostic_task_handle_ge = NULL;



QueueHandle_t os_uart_rx_queue_handler_ge   = NULL;

QueueHandle_t os_uart_tx_queue_handler_ge   = NULL;



SemaphoreHandle_t uart_rx_intrupt_Semaphore = NULL;

SemaphoreHandle_t uart_tx_intrupt_Semaphore = NULL;

SemaphoreHandle_t can_rx_intrupt_Semaphore = NULL;

SemaphoreHandle_t can_tx_intrupt_Semaphore = NULL;




//TODO : Do a stack memory analysis to optimize the RAM space
#define OS_UART_RX_TASK_STACK_SIZE		(256U) /* 256 words = 1024B - 744 Used */
#define OS_UART_TX_TASK_STACK_SIZE		(256U) /* 256 words = 1024B - 744 Used */
#define OS_CAN_RX_TASK_STACK_SIZE		(256U)
#define OS_CAN_TX_TASK_STACK_SIZE		(256U)
#define OS_DIAGNOSTIC_TASK_STACK_SIZE		(256U)



/* Sytem monitoring and configuration */
/* Measurement of all the raw data */














/* Public Function Definitions */
uint8_t operating_system_init_u8(void);
void operating_system_start_scheduler_v(void);

/**
=====================================================================================================================================================

@fn Name	    :operating_system_init_uint8_t
@b Scope            :public
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
uint8_t operating_system_init_u8(void)
{
	uint8_t return_val_u8 = 0;

	//bootsm_boot_status_e = BOOTSM_SYS_OS_INIT;

	if (operating_system_create_semaphore_pru8())
	{
		return_val_u8 += 1;

	}
	if(operating_system_create_queue_pru8())
	{
		return_val_u8 += 1;
	}
	if (operating_system_create_task_pru8())
	{
		return_val_u8 += 1;

	}

	return (return_val_u8);
}

/**
=====================================================================================================================================================

@fn Name	    :operating_system_start_scheduler_v
@b Scope            :Public
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
void operating_system_start_scheduler_v(void)
{
	//bootsm_boot_status_e = BOOTSM_SYS_OS_START_KERNAL;

	/*start tasks*/
	vTaskStartScheduler();
}
/**
=====================================================================================================================================================

@fn Name	    :operating_system_create_semaphore_pru8
@b Scope            : Private
@n@n@b Description  :Creates all required Semaphore flags
@param Input Data   :void
@return Return Value:U8

=====================================================================================================================================================
*/
static uint8_t operating_system_create_semaphore_pru8(void)
{
	uint8_t return_val_u8 = 0;

	uart_rx_intrupt_Semaphore = xSemaphoreCreateBinary();
	if(uart_rx_intrupt_Semaphore == NULL)
	{
	   return_val_u8++;
	}
	uart_tx_intrupt_Semaphore = xSemaphoreCreateBinary();
	if(uart_tx_intrupt_Semaphore == NULL)
	{
		 return_val_u8++;
	}
	can_rx_intrupt_Semaphore = xSemaphoreCreateBinary();
	if(can_rx_intrupt_Semaphore == NULL)
	{
		return_val_u8++;
	}
	return (return_val_u8);
}

/**
=====================================================================================================================================================

@fn Name	    :operating_system_create_queue_pru8
@b Scope            : Private
@n@n@b Description  :Creates all required queue
@param Input Data   :void
@return Return Value:uint8_t

=====================================================================================================================================================
*/
static uint8_t operating_system_create_queue_pru8(void)
{
	uint8_t return_val_u8 = 0;

	os_uart_rx_queue_handler_ge = xQueueCreate(OS_UART_RX_QUEUE_SIZE, sizeof(operating_system_uart_rx_queue_tst));
	if (os_uart_rx_queue_handler_ge == NULL)
	{
		return_val_u8++;
	}
	os_uart_tx_queue_handler_ge = xQueueCreate(OS_UART_RX_QUEUE_SIZE, sizeof(operating_system_uart_rx_queue_tst));
	if (os_uart_tx_queue_handler_ge == NULL)
	{
		return_val_u8++;
	}

	return (return_val_u8);
}

/**
=====================================================================================================================================================

@fn Name	    :operating_system_create_mutex_pru8
@b Scope            : Private
@n@n@b Description  :
@param Input Data   :void
@return Return Value:uint8_t

=====================================================================================================================================================
*/
#if 0
static uint8_t operating_system_create_mutex_pru8(void)
{
	uint8_t return_val_u8 = 0;


  return (return_val_u8);
}

#endif
/**
=====================================================================================================================================================

@fn Name	    :operating_system_create_task_pru8
@b Scope            : Private
@n@n@b Description  :
@param Input Data   :void
@return Return Value:uint8_t

=====================================================================================================================================================
*/
static uint8_t operating_system_create_task_pru8(void)
{
	uint8_t return_val_u8 = 0;

	/* definition and creation of bmsTask */

#if 1
	xTaskCreate(uart_rx_task_v,      /* The function that implements the task. */
	"uart_rx_task", 									 /* The text name assigned to the task - for debug only as it is not used by the kernel. */
	OS_UART_RX_TASK_STACK_SIZE,                   /* The size of the stack to allocate to the task. */
	NULL, 										 /* The parameter passed to the task - just to check the functionality. */
	OS_UART_RX_TASK_PRIORITY, 					 /* The priority assigned to the task. */
	&os_uart_rx_task_handle_ge); 					 /* The task handle is not required, so NULL is passed. */

	if (os_uart_rx_task_handle_ge == NULL)
	{
		return_val_u8++;
	}

#endif

#if 1
	/* definition and creation of bmsTask */
    xTaskCreate(uart_tx_task_v,					/* The function that implements the task. */
				"uart_tx_task",									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				OS_UART_TX_TASK_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL,                              		/* The parameter passed to the task - just to check the functionality. */
				OS_UART_TX_TASK_PRIORITY, 					/* The priority assigned to the task. */
				&os_uart_tx_task_handle_ge);				/* The task handle is not required, so NULL is passed. */

	if (os_uart_tx_task_handle_ge == NULL)
	{
		return_val_u8++;
	}
#endif

#if 1
	/* definition and creation of bmsTask */
    xTaskCreate(can_comm_rx_task_v,					/* The function that implements the task. */
				"can_rx_task",									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				OS_CAN_RX_TASK_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL,                              		/* The parameter passed to the task - just to check the functionality. */
				OS_CAN_RX_TASK_PRIORITY, 					/* The priority assigned to the task. */
				&os_can_rx_task_handle_ge);				/* The task handle is not required, so NULL is passed. */

	if (os_can_rx_task_handle_ge == NULL)
	{
		return_val_u8++;
	}
#endif

#if 0
	/* definition and creation of bmsTask */
    xTaskCreate(can_comm_tx_task_v,					/* The function that implements the task. */
				"bms_can_tx",									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				OS_CAN_TX_TASK_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL,                              		/* The parameter passed to the task - just to check the functionality. */
				OS_CAN_TX_TASK_PRIORITY, 					/* The priority assigned to the task. */
				&os_can_tx_task_handle_ge);				/* The task handle is not required, so NULL is passed. */

	if (os_can_tx_task_handle_ge == NULL)
	{
		return_val_u8++;
	}
#endif

#if 1
	/* definition and creation of bmsTask */
    xTaskCreate(diagnostic_task_v,					/* The function that implements the task. */
				"bms_can_tx",									/* The text name assigned to the task - for debug only as it is not used by the kernel. */
				OS_DIAGNOSTIC_TASK_STACK_SIZE, 				/* The size of the stack to allocate to the task. */
				NULL,                              		/* The parameter passed to the task - just to check the functionality. */
				OS_DIAGNOSTIC_TASK_PRIORITY, 					/* The priority assigned to the task. */
				&os_diagnostic_task_handle_ge);				/* The task handle is not required, so NULL is passed. */

	if (os_diagnostic_task_handle_ge == NULL)
	{
		return_val_u8++;
	}
#endif


	return (return_val_u8);
}

/****************************************************************************************************************************************************
 * EOF
 ***************************************************************************************************************************************************/
