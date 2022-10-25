/**
========================================================================================================================
@page 

@subsection  Details

@n@b Title:      						
@n@b Filename:   	internal_flash_app.h		
@n@b MCU:       	S32K144					
@n@b Compiler:   						
@n@b Author:     	Prabhu					

------------------------------------------------------------------------------------------------------------------------
@subsection  Description
@b Description:

------------------------------------------------------------------------------------------------------------------------
@subsection MISRA Exceptions
@b MISRA @b Exceptions:

------------------------------------------------------------------------------------------------------------------------
@subsection Notes
@b Notes:

------------------------------------------------------------------------------------------------------------------------

@n@b Rev_by:   	#1_TL#			
@n@b Date:       	06-Jul-2022	
@n@b Description:  	Created			

------------------------------------------------------------------------------------------------------------------------
(C) Copyright World of River. All Rights Reserved.
------------------------------------------------------------------------------------------------------------------------

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

========================================================================================================================
*/
#ifndef _INTERNAL_FLASH_APP_H_
#define _INTERNAL_FLASH_APP_H_

#include "common_header.h"
#include "internal_flash.h"





/** @} */
/** @{
 *  Public Variable Definitions */




/** @} */

/* Public Function Prototypes **/
/** Write Single Data**/
internal_flash_status_e internal_flash_app_write_one_data_e(U32 flash_start_address_aru32,U8 flash_data_aru8);
/** Read Single Data**/
internal_flash_status_e internal_flash_app_read_one_data_e(U32 flash_start_address_aru32,U32* flash_data_arpu32);
/** Write N No. OF Data**/
internal_flash_status_e internal_flash_app_write_e(U32 flash_write_start_address_aru32,
                                             U8* flash_data_arpu8,U32 flash_data_size_aru32);
/**  Read N No. OF Data**/
internal_flash_status_e internal_flash_app_read_e(U32 flash_read_start_address_aru32,U8* flash_read_data_arpu8,
                                            U32 flash_read_data_size_aru32);
/**Erase internal flash memory withn default user start address and user  end address **/
internal_flash_status_e in_flash_app_full_erase_e(void);
/**Erase internal flash memory with  internal_flash_start_address_u32 and internal_flash_end_address_u32 **/
internal_flash_status_e in_flash_app_erase_e(U32 internal_flash_start_address_aru32,U32 internal_flash_end_address_aru32);
extern U8 internal_flash_app_init(void);

#endif /* CONFIGURATION_LAYER_INC_DEVICE_DRIVER_API_INTERNAL_FLASH_APP_H_ */
