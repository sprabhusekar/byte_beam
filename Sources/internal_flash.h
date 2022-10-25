/**
========================================================================================================================
@page 

@subsection  Details

@n@b Title:      						
@n@b Filename:   	rr_internal_flash.h		
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
#ifndef CORE_LAYER_INC_RR_INTERNAL_FLASH_H_
#define CORE_LAYER_INC_RR_INTERNAL_FLASH_H_

#define INTERNAL_FLASH_START_ADDRESS            ((U32)0x00000000)
#define INTERNAL_FLASH_END_ADDRESS              ((U32)0x0007C000)
#define INTERNAL_FLASH_USER_START_ADDRESS       ((U32)0x00000000)
#define INTERNAL_FLASH_USER_END_ADDRESS         ((U32)0x0007C000)
#define INTERNAL_FLASH_PAGE_SIZE                ((U32)4096)
#define INTERNAL_FLASH_32_BIT_ADDRESS_COUNT     8
#define INTERNAL_FLASH_64_BIT_ADDRESS_COUNT     8
#define INTERNAL_FLASH_32_BIT_DATA_COUNT        8
#define INTERNAL_FLASH_DATA_FORMAT              FLASH_TYPEPROGRAM_DOUBLEWORD
#define INTERNAL_FLASH_ERASE_TYPE               FLASH_TYPEERASE_PAGES
#define INTERNAL_FLASH_BANK_TYPE                FLASH_BANK_1

/** @{
 * Type definition */
typedef enum
{
	 IN_FLASH_SUCCESS  = 0x00U,
	IN_FLASH_UNKNOWN  = 0x01U,
    IN_FLASH_ERROR    = 0x02U,
    IN_FLASH_BUSY     = 0x03U,
    IN_FLASH_TIMEOUT  = 0x04U,
    IN_FLASH_ADDRESS_ERROR = 0x05,
	EXT_FLASH_READ_ERROR

}internal_flash_status_e;





/** @} */
/** @{
 *  Public Variable Definitions */




/** @} */

/* Public Function Prototypes **/
extern U8 internal_flash_init(void);
extern internal_flash_status_e internal_flash_program_e(U32 flash_start_address_aru32,U16 write_count_u16,U8 *flash_data_arpu8);
extern internal_flash_status_e internal_flash_erase_e(U32 flash_erase_start_address_aru32 ,U32 flash_erase_end_address_aru32 );

#endif /* CORE_LAYER_INC_RR_INTERNAL_FLASH_H_ */
