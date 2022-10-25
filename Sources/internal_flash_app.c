/**
====================================================================================================================================================================================
@page can_comm_task_source_file

@subsection  Details

@n@b Title:      						
@n@b Filename:   	internal_flash_app.c	
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




/* Driver Header File */

#include "internal_flash.h"
#include "osif.h"
#include "interrupt_manager.h"
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





/**
=====================================================================================================================================================

@fn Name			: internal_flash_app_write_one_data_e
@b Scope            : Public
@n@n@b Description  : Write Single Data into internal Flash Memory
@param Input Data   : flash_start_address_aru32 & flash_data_aru8
@return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

=====================================================================================================================================================
*/
internal_flash_status_e internal_flash_app_write_one_data_e(U32 flash_start_address_aru32,U8 flash_data_aru8)
{
    internal_flash_status_e in_flash_write_status = IN_FLASH_SUCCESS;
    /** Write Single Data into Internal Flash Memory **/
   in_flash_write_status = internal_flash_program_e(flash_start_address_aru32, 8,&flash_data_aru8);
   return in_flash_write_status;
}

/**
=====================================================================================================================================================

@fn Name			: internal_flash_app_read_one_data_e
@b Scope            : Public
@n@n@b Description  : Read Single Data from  internal Flash Memory
@param Input Data   : flash_start_address_aru32 & flash_data_arpu32
@return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

=====================================================================================================================================================
*/
 internal_flash_status_e internal_flash_app_read_one_data_e(U32 flash_start_address_aru32,U32* flash_data_arpu32)
{
    internal_flash_status_e in_flash_read_status_e = IN_FLASH_SUCCESS;
    /** read  Single Data from Internal Flash Memory **/
    *flash_data_arpu32= *(volatile U32 *)flash_start_address_aru32;
   return in_flash_read_status_e;
}

 /**
 =====================================================================================================================================================

 @fn Name			 : internal_flash_app_write_e
 @b Scope            : Public
 @n@n@b Description  : Write N no.of  Data into internal Flash Memory
 @param Input Data   : flash_write_start_address_aru32 , flash_data_arpu8, flash_data_size_aru32
 @return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

 =====================================================================================================================================================
 */
internal_flash_status_e internal_flash_app_write_e(U32 flash_write_start_address_aru32,
                                             U8* flash_data_arpu8,U32 flash_data_size_aru32)
{
       U32 data_size_count_u32 = 0;
       U16 flash_data_write_count_u16 =0;
       flash_data_size_aru32 = (flash_data_size_aru32/INTERNAL_FLASH_32_BIT_DATA_COUNT)+
                              ((flash_data_size_aru32%INTERNAL_FLASH_32_BIT_DATA_COUNT)>0?1:0);
       internal_flash_status_e in_flash_write_status_e = IN_FLASH_UNKNOWN;
       if(INTERNAL_FLASH_USER_END_ADDRESS < flash_write_start_address_aru32 )
       {
    	   in_flash_write_status_e = IN_FLASH_ADDRESS_ERROR;
           return in_flash_write_status_e;
       }
       INT_SYS_DisableIRQGlobal();
       /** Write Single Data into Internal Flash Memory **/
       for(data_size_count_u32 = 0;data_size_count_u32 < flash_data_size_aru32 ; data_size_count_u32++)
       {
           /** Checking flash start address exceed or not**/
           if((INTERNAL_FLASH_END_ADDRESS-INTERNAL_FLASH_64_BIT_ADDRESS_COUNT) >= flash_write_start_address_aru32 )
           {
               /** Write Single Data into Internal Flash Memory **/
        	   in_flash_write_status_e  = internal_flash_program_e(flash_write_start_address_aru32,INTERNAL_FLASH_64_BIT_ADDRESS_COUNT,
        			   	   	   	   	   &flash_data_arpu8[flash_data_write_count_u16]) ;
               if(IN_FLASH_SUCCESS == in_flash_write_status_e  )
               {
            	   flash_write_start_address_aru32 = flash_write_start_address_aru32 + INTERNAL_FLASH_64_BIT_ADDRESS_COUNT ;
                   flash_data_write_count_u16 += INTERNAL_FLASH_64_BIT_ADDRESS_COUNT;
                   in_flash_write_status_e = IN_FLASH_SUCCESS;

               }
               else
               {
            	   in_flash_write_status_e = IN_FLASH_ADDRESS_ERROR;
            	   INT_SYS_EnableIRQGlobal();
                   return in_flash_write_status_e;
               }
          }
          else
          {
        	  in_flash_write_status_e = IN_FLASH_ADDRESS_ERROR;
        	  INT_SYS_EnableIRQGlobal();
              return in_flash_write_status_e;
          }
       }
      INT_SYS_EnableIRQGlobal();
      return in_flash_write_status_e;
}

/**
=====================================================================================================================================================

@fn Name			: internal_flash_app_read_e
@b Scope            : Public
@n@n@b Description  : Read N no.of  Data from internal Flash Memory
@param Input Data   : flash_read_start_address_aru32 , flash_read_data_arpu8, flash_read_data_size_aru32
@return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

=====================================================================================================================================================
*/
internal_flash_status_e internal_flash_app_read_e(U32 flash_read_start_address_aru32,U8* flash_read_data_arpu8,
                                            U32 flash_read_data_size_aru32)
{
       U32 data_size_count_u32 = 0;
       internal_flash_status_e in_flash_read_status_e = IN_FLASH_SUCCESS;
       /*flash_read_data_size_u32 = (flash_read_data_size_u32/INTERNAL_FLASH_32_BIT_DATA_COUNT)+
                                     ((flash_read_data_size_u32%INTERNAL_FLASH_32_BIT_DATA_COUNT)>0?1:0);*/
       if(INTERNAL_FLASH_USER_END_ADDRESS < flash_read_start_address_aru32 )
       {
    	   in_flash_read_status_e = IN_FLASH_ADDRESS_ERROR;
           return in_flash_read_status_e;
       }
       /** read Data from Internal Flash Memory **/
       for(data_size_count_u32 = 0;data_size_count_u32 < flash_read_data_size_aru32 ; data_size_count_u32++)
       {
           /** Checking flash start address exceed or not**/
           if((INTERNAL_FLASH_END_ADDRESS- INTERNAL_FLASH_64_BIT_ADDRESS_COUNT) >= flash_read_start_address_aru32 )
           {
               /** Read Single Data into Internal Flash Memory **/
        	   flash_read_data_arpu8[data_size_count_u32] = *(volatile U32 *)flash_read_start_address_aru32;
               /*** Address increment based ON data size in Flash **/
               flash_read_start_address_aru32++;
           }
           else
           {
        	   in_flash_read_status_e = IN_FLASH_ADDRESS_ERROR;
              return in_flash_read_status_e;
           }
       }
      return in_flash_read_status_e;
}

/**
=====================================================================================================================================================

@fn Name			: in_flash_app_full_erase_e
@b Scope            : Public
@n@n@b Description  : Erase all data from internal flash
@param Input Data   : NULL
@return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

=====================================================================================================================================================
*/
internal_flash_status_e in_flash_app_full_erase_e(void)
{
    internal_flash_status_e in_flash_erase_status_e = IN_FLASH_SUCCESS;
    in_flash_erase_status_e = internal_flash_erase_e(INTERNAL_FLASH_USER_START_ADDRESS,INTERNAL_FLASH_USER_END_ADDRESS);
    return in_flash_erase_status_e;
}

/**
=====================================================================================================================================================

@fn Name			: in_flash_app_erase_e
@b Scope            : Public
@n@n@b Description  : Erase all data from internal_flash_start_address_aru32 to internal_flash_end_address_aru32
@param Input Data   : internal_flash_start_address_aru32 , internal_flash_end_address_aru32
@return Return Value: internal_flash_status_e Zero if OK , Non-Zero if NOT OK

=====================================================================================================================================================
*/
internal_flash_status_e in_flash_app_erase_e(U32 internal_flash_start_address_aru32,U32 internal_flash_end_address_aru32)
{

    internal_flash_status_e in_flash_erase_status_e = IN_FLASH_SUCCESS;
    INT_SYS_DisableIRQGlobal();
    in_flash_erase_status_e = internal_flash_erase_e(internal_flash_start_address_aru32,internal_flash_end_address_aru32);
    INT_SYS_EnableIRQGlobal();
    return in_flash_erase_status_e;
}

/**
=====================================================================================================================================================

@fn Name			:
@b Scope            : Public
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/
U8 internal_flash_app_init(void)
{
	U8  in_flsh_init_sts_u8 = 3;
	in_flsh_init_sts_u8 = internal_flash_init();
	return  in_flsh_init_sts_u8;

}




