/**
 * @file mw_memory.c
 * @brief Contains all the drivers related to Memory peripheral.
 */

/* Common Header Files */
#include "common_header.h"

/* Application Header File */

/* This header file */
#include "memory.h"

/* Driver header */
/** @{
 * Private Definitions **/

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

/** @} */

/* Public Function Definitions */
 void memory_copy_u8_array_v(U8* destination_arg_pu8, U8* source_arg_pu8, U32 bytes_to_copy_arg_u32);
 void memory_set_u8_array_v(U8* destination_arg_pu8, U8 value_arg_u8, U32 bytes_to_copy_arg_u32);

/**
=====================================================================================================================================================

@fn Name			:
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
 void memory_copy_u8_array_v(U8* destination_arg_pu8, U8* source_arg_pu8, U32 bytes_to_copy_arg_u32)
{
	U32 index_u32 = 0;

	for(;index_u32 < bytes_to_copy_arg_u32; index_u32++)
	{
	    destination_arg_pu8[index_u32] = source_arg_pu8[index_u32];
	}
}

/**
=====================================================================================================================================================

@fn Name			:
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
 void memory_set_u8_array_v(U8* destination_arg_pu8, U8 value_arg_u8, U32 bytes_to_copy_arg_u32)
{
	U32 index_u32 = 0;

	for(;index_u32 < bytes_to_copy_arg_u32; index_u32++)
	{
		destination_arg_pu8[index_u32] = value_arg_u8;
	}
}

/**
=====================================================================================================================================================

@fn Name			:
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
U8 memory_comp_u8(U8 *val1_arpu8, U8 *val2_arpu8, U32 bytes_to_compare_aru32)
{
	U32 index_u32 = 0;

	for(;index_u32 < bytes_to_compare_aru32; index_u32++)
	{
		if(val1_arpu8[index_u32] != val2_arpu8[index_u32])
		{
			return 1;			/* Failure due to compared values are not same */
		}
	}

	return 0;	/* compared values are same */
}


/****************************************************************************************************************************************************
 * EOF
 ***************************************************************************************************************************************************/
