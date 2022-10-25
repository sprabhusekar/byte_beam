/**
====================================================================================================================================================================================
@page can_comm_task_source_file

@subsection  Details

@n@b Title:      						
@n@b Filename:   	rr_internal_flash.c	
@n@b MCU:       	S32K144					
@n@b Compiler:   	GCC					
@n@b Author:     	Prabhu					

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@subsection  Description
@b Description:

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@subsection MISRA Exceptions
@b MISRA @b Exceptions:

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
@subsection Notes
@b Notes:

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

@n@b Rev_by:   		#1_TL#			
@n@b Date:       	06-Jul-2022	
@n@b Description:  	Created			

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
(C) Copyright World of River. All Rights Reserved.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

@b DISCLAIMERS:
- This software is for development purposes only, and is not suitable for
production release until verified and validated by the end customer.
- This software is provided "as is". No warranties apply, whether express,
implied or statutory, including but not limited to implied warranties of
merchantability and fitness for a particular purpose.
- World of River reserves the right, without notice, to make changes to this
software.
- World of River is not, in any circumstances, liable for special, incidental,
or consequential damages for any reason whatsoever arising out of the use or
application of this software.

====================================================================================================================================================================================

*/

/* Common Header Files */
#include "common_header.h"



/* Application Header File */




/* Configuration Header File */




/* This header file */
#include "internal_flash.h"



/* Driver Header File */
#include "Flash1.h"


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


/* Declare a FLASH config struct which initialized by FlashInit, and will be used by all flash operations */
flash_ssd_config_t flashSSDConfig;


/** @} */


/* Public Function Definitions */




/**
=====================================================================================================================================================

@fn Name	    :
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
U8 internal_flash_init(void)
{
	U8 int_flash_init_sts_u8 = 0;
	if(FLASH_DRV_Init(&internal_flash_config_st, &flashSSDConfig) == 0)
	{
		int_flash_init_sts_u8 = 0;
	}
	else
	{
		int_flash_init_sts_u8++;
	}
	return int_flash_init_sts_u8;

}

/**
 =====================================================================================================================================================

 @fn Name			 : internal_flash_program_e
 @b Scope            : Public
 @n@n@b Description  : write data in internal flash memory
 @param Input Data   : flash_start_address_aru32, flash_data_arpu8
 @return Return Value: internal_flash_status_e -	Zero if OK, Non-Zero if NOT OK

 =====================================================================================================================================================
 */
internal_flash_status_e internal_flash_program_e(U32 flash_start_address_aru32,U16 write_count_u16,U8 *flash_data_arpu8)
{
    internal_flash_status_e  internal_flash_program_status_e  = IN_FLASH_SUCCESS;
    /*  write data in internal flash memeory */
    if(FLASH_DRV_Program(&flashSSDConfig,
    		 flash_start_address_aru32,write_count_u16,flash_data_arpu8) != 0)
    {
    	internal_flash_program_status_e = IN_FLASH_ERROR;
    }
    return  internal_flash_program_status_e;
}

/**
=====================================================================================================================================================

@fn Name			: internal_flash_erase_e
@b Scope            : Public
@n@n@b Description  : Erase all data from flash_erase_start_address_aru32 to flash_erase_end_address_aru32
@param Input Data   : flash_erase_start_address_aru32, flash_erase_end_address_aru32
@return Return Value: internal_flash_status_e -	Zero if OK, Non-Zero if NOT OK

=====================================================================================================================================================
*/
internal_flash_status_e internal_flash_erase_e(U32 flash_erase_start_address_aru32 ,U32 flash_erase_end_address_aru32 )
{
	/*U32 status_clear_u32; */
    internal_flash_status_e  internal_flash_program_status_e  = IN_FLASH_UNKNOWN;
   /* U32 internal_flash_no_of_page_u32 = 0; */
    /* Define the number of page to be erased */
    /*internal_flash_no_of_page_u32 = (flash_erase_end_address_aru32 - flash_erase_start_address_aru32)
                                             / INTERNAL_FLASH_PAGE_SIZE;
    */
    while(flash_erase_start_address_aru32 <= flash_erase_end_address_aru32)
    {
    	if(FLASH_DRV_EraseSector(&flashSSDConfig,
    							flash_erase_start_address_aru32, INTERNAL_FLASH_PAGE_SIZE) == 0)
    	 {
    		 flash_erase_start_address_aru32 += INTERNAL_FLASH_PAGE_SIZE;
    		 internal_flash_program_status_e = IN_FLASH_SUCCESS;
    	 }
    	 else
    	 {
    		 internal_flash_program_status_e = IN_FLASH_ERROR;
    		 return internal_flash_program_status_e;
    	 }
     }
    return  internal_flash_program_status_e;
}



