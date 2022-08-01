#ifndef CORE_LAYER_INC_RR_NVIC_H_
#define CORE_LAYER_INC_RR_NVIC_H_

/* Includes for definitions */
/* Includes for definitions */

#include "S32K144.h"

/* @{
 * Macro definition */
typedef enum nvic_irq_state_te_tag
{
    NVIC_IRQ_DISABLE = 0,
    NVIC_IRQ_ENABLE = 1

}nvic_irq_state_te;

typedef enum nvic_irq_lock_te_tag
{
    NVIC_IRQ_LOCK_DISABLE = 0,
    NVIC_IRQ_LOCK_ENABLE = 1

}nvic_irq_lock_te;

typedef struct nvic_initstruct_st_tag
{
    IRQn_Type irq_num_e;
    uint8_t priority_u8;
    nvic_irq_lock_te preos_irqlock_e;
    nvic_irq_state_te state_e;
} nvic_initstruct_st;

/* @} */

/* @{
 * Type definition */

/* @} */

/* @{
 *  Public Variable Definitions */

/* @} */

/* Public Function Prototypes **/
extern void nvic_init_v(void);
extern void nvic_enable_all_Interrupts_v(void);
extern void nvic_disable_all_Interrupts_v(void);
extern void nvic_enable_interrupt_v(IRQn_Type interrupt_arg_e);
extern void nvic_disable_interrupt_v(IRQn_Type interrupt_arg_e);

#endif /* CORE_LAYER_NVIC_H_ */
