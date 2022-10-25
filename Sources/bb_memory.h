/**
 * @file mw_memory.h
 * @brief Contains all the drivers related to Memory peripheral.
 */

#ifndef INCLUDES_CORE_LAYER_INC_RR_MEMORY_H_
#define INCLUDES_CORE_LAYER_INC_RR_MEMORY_H_
#include "common_header.h"
/* Includes for definitions */

/** @{
 * Type definition */

/** @} */
/** @{
 *  Public Variable Definitions */

/** @} */

/* Public Function Prototypes **/
extern void memory_copy_u8_array_v(U8* destination_arg_pu8, U8* source_arg_pu8, U32 bytes_to_copy_arg_u32);
extern void memory_set_u8_array_v(U8* destination_arg_pu8, U8 value_arg_u8, U32 bytes_to_copy_arg_u32) ;
extern U8 memory_comp_u8(U8 *val1_arpu8, U8 *val2_arpu8, U32 bytes_to_compare_aru32);


#endif /* INCLUDES_CORE_LAYER_MW_MEMORY_H_ */
