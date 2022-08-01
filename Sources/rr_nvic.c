
/* Common Header Files */


/* Application Header File */


/* Configuration Header File */

/* This header file */
#include "rr_nvic.h"

/* Driver Header File */
#include "interrupt_manager.h"
#include "S32K144.h"
#include "device_registers.h"
#include "clock_manager.h"
/** @{
 * Private Definitions **/
uint8_t nvic_num_of_slots_configured_gu8 = 0;
/** @} */

/* Private Function Prototypes */

/** @{
 * Type definition */

/** @} */


/** @{
 * Private Variable Definitions */

/** @} */

/** @{
 *  Public Variable Definitions */
nvic_initstruct_st nvic_init_st[] =
{
	/* DMA 15 dma channels available*/
   /*{ DMA0_IRQn, 			     3, NVIC_IRQ_LOCK_ENABLE, 	NVIC_IRQ_DISABLE },
	{ DMA1_IRQn, 				 3, NVIC_IRQ_LOCK_ENABLE, 	NVIC_IRQ_DISABLE },
	{ DMA2_IRQn, 				 3, NVIC_IRQ_LOCK_ENABLE, 	NVIC_IRQ_DISABLE },
	{ DMA3_IRQn, 				 3, NVIC_IRQ_LOCK_ENABLE, 	NVIC_IRQ_DISABLE },
	{ DMA_Error_IRQn,            3, NVIC_IRQ_LOCK_ENABLE, 	NVIC_IRQ_DISABLE }, */

	/* CAN1 Interrupts */
	{ CAN0_ORed_0_15_MB_IRQn, 	4, NVIC_IRQ_LOCK_ENABLE,   NVIC_IRQ_ENABLE  },
	{ CAN0_ORed_16_31_MB_IRQn, 	4, NVIC_IRQ_LOCK_ENABLE,   NVIC_IRQ_ENABLE  },
	{ CAN0_Error_IRQn, 	 		4, NVIC_IRQ_LOCK_ENABLE,   NVIC_IRQ_ENABLE  },
	{ CAN0_Wake_Up_IRQn, 	 	4, NVIC_IRQ_LOCK_ENABLE,   NVIC_IRQ_ENABLE  },
	/* TIMER Interrupts */
	{ FTM0_Ch0_Ch1_IRQn,              5, NVIC_IRQ_LOCK_ENABLE,  NVIC_IRQ_ENABLE },
	{LPUART0_RxTx_IRQn,3,NVIC_IRQ_LOCK_ENABLE,NVIC_IRQ_ENABLE}

};

/** @} */


/* Public Function Definitions */
void nvic_init_v(void);
void nvic_enable_all_Interrupts_v(void);
void nvic_disable_all_Interrupts_v(void);
void nvic_enable_interrupt_v(IRQn_Type interrupt_arg_e);
void nvic_disable_interrupt_v(IRQn_Type interrupt_arg_e);

/**
=====================================================================================================================================================

@fn Name	    :
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
void nvic_init_v(void)
{
	 uint8_t index_u8 = 0;

	    /* Get the number of interrupts to be configured from the structure table */
	    nvic_num_of_slots_configured_gu8 = sizeof(nvic_init_st) / sizeof(nvic_init_st[0]);

	    /* Disable Interrupts */
	    for (index_u8 = 0; index_u8 < FTM3_Ovf_Reload_IRQn; index_u8++)
		{
	    	INT_SYS_DisableIRQ(index_u8);
		}

	    /* Set Interrupt priority */
	    for (index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	    {
	    	INT_SYS_SetPriority(nvic_init_st[index_u8].irq_num_e, nvic_init_st[index_u8].priority_u8);
	    }

	    /* Enable Interrupts - Unlocked */
	    for (index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	    {
			if (nvic_init_st[index_u8].preos_irqlock_e == NVIC_IRQ_LOCK_DISABLE)
			{
				if (nvic_init_st[index_u8].state_e == NVIC_IRQ_ENABLE)
				{
					INT_SYS_EnableIRQ(nvic_init_st[index_u8].irq_num_e);
				}
			}
	    }
}


void nvic_enable_interrupt_v(IRQn_Type interrupt_arg_e)
{
	for (uint8_t index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	    {
	        if (nvic_init_st[index_u8].irq_num_e == interrupt_arg_e)
	        {
	        	if (nvic_init_st[index_u8].state_e == NVIC_IRQ_ENABLE)
	        	{
	        		INT_SYS_EnableIRQ(interrupt_arg_e);
	        	}
	            break;
	        }
	    }
}
void nvic_disable_interrupt_v(IRQn_Type interrupt_arg_e)
{
	for (uint8_t index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	{
		if (nvic_init_st[index_u8].irq_num_e == interrupt_arg_e)
		{
			INT_SYS_DisableIRQ(interrupt_arg_e);
			break;
		}
	}
}

void nvic_enable_all_Interrupts_v(void)
{
	for (uint8_t index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	{
		if (nvic_init_st[index_u8].state_e == NVIC_IRQ_ENABLE)
		{
			INT_SYS_EnableIRQ(nvic_init_st[index_u8].irq_num_e);
		}
	}
}

void nvic_disable_all_Interrupts_v(void)
{
	for (uint8_t index_u8 = 0; index_u8 < nvic_num_of_slots_configured_gu8; index_u8++)
	{
		INT_SYS_DisableIRQ(nvic_init_st[index_u8].irq_num_e);
	}
}

void nvic_HardFault_Handler(void)
{

	while(1);
}

void nvic_NMI_Handler(void)
{

	while(1);
}

/****************************************************************************************************************************************************
 * EOF
 ***************************************************************************************************************************************************/
