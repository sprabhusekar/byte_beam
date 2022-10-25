/* Common Header Files */

/* Application Header File */




#include "rr_nvic.h"
#include "timer.h"
#include "can.h"
#include "system_S32K144.h"
#include "bootloader_app.h"
#include "internal_flash_app.h"
#include "bb_memory.h"
#include "uart_state_machine.h"
#include "uart.h"

/* Driver header */

#define INTERNAL_FLASH_BLOCK_SIZE 1024


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
U8 can_bootloader_cmd_received_gvlu8 = 0;
bootloader_config_tst boot_loader_conf_gst;
bootloader_config_tst read_boot_loader_conf_gst;
/*can_app_message_tst bapp_comm_rx_msg_st;
can_app_message_tst bapp_comm_tx_msg_st;*/

U8 bootloader_app_can_rx_local_buffer_au8[8];
U32 boot_loader_data_rec_count_u32 = 0;
U32 total_boot_loader_data_received_u32 = 0;
U32 crc_check_sum_u32 = 0;

U8 boot_loader_flash_data_au8[10];
U8 read_boot_loader_buffer_au8[10];
U8 bootloader_checksum_byte_au8[8];
U32 data_reset_count_u32 = 0;
U8 bootloading_process_enabled_u8 = 0;
U32 bootloader_processing_timer_u32 = 0;
U32 start_boot_loader_write_address_u32 = NEW_APPLICATION_BACKUP_START_ADDRESS;
U32 start_boot_loader_read_address_u32 = NEW_APPLICATION_BACKUP_START_ADDRESS;

U32 JumpAddress = 0;
U32 app_entry_u32 = 0;
typedef void (*pFunction_1)(void);
pFunction_1 JumpToApplication;

bootlaoder_PGP_key_st bootloader_diag_tool_pgp_key_st;

bootlaoder_diag_tool_ack_st bootloader_diag_tool_ack_st;

bootloader_config_tst received_boot_config_st;
bootloader_config_tst flash_read_boot_config_st;

U32 int_flash_write_data_u32 = 0;

volatile U8 bootloader_failed_vu8 = 0;



bootloader_uart_app_list_te uart_bootloader_state_ge;
/** @} */

/* Public Function Definitions */

U32 bootloader_app_crc32_generate_u32(U8 *data_arpu8, U32 length_aru32);
U32 bootloader_app_whole_bin_crc32_generate_u32(U32 application_address_u32, U32 length_aru32);
bootloader_app_sts_te boot_loader_app_check_new_firmware_e(void);
void bootloader_internal_application_flash_erase_u8(void);
void bootloader_app_failure_v(void);
void bootloader_update_new_firmware(void);
bootloader_app_sts_te boot_loader_app_run_old_firmware_e(void);
 void bootloader_enable_cmd_procees(void);
 void bootloader_new_firmware_receive(void);
 void bootloader_whole_crc_check(void);
 void bootloader_end_of_data_handling(void);
 void bootloader_jumping_to_boot_section(void);

static void  bootlaoder_erase_application_section(void);

U8 boot_send_one_second_sts_flg_u8 = 0;

U32 backup_app_start_adrs_u32 = 0;
	U32 firmware_write_adrs_u32  = 0;
	U32 firmware_write_count_u32 = 0;
	U32 app_crc_u32 =0;


/**
=====================================================================================================================================================

@fn Name			:
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/

U32 bootloader_app_crc32_generate_u32(U8 *data_arpu8, U32 length_aru32)
{
	U32 crc_u32 = 0 ;
	S8 i = 0;
	crc_u32 = ~crc_u32;
	  while (length_aru32 > 0)
	  {
	      crc_u32 = crc_u32 ^ *data_arpu8++;
	    for (i = 8; --i >= 0;)
	    {
	        crc_u32 = (crc_u32 >> 1) ^ (0xedb88320 & -(crc_u32 & 1));
	    }
	    length_aru32--;
	  }
	  return ~crc_u32;
}

/**
=====================================================================================================================================================

@fn Name            :
@b Scope            :
@n@n@b Description  :
@param Input Data   :
@return Return Value:

=====================================================================================================================================================
*/

U32 bootloader_app_whole_bin_crc32_generate_u32(U32 application_address_u32, U32 length_aru32)
{
    U32 crc_u32 = 0 ;
    S8 i = 0;
    crc_u32 = ~crc_u32;
      while (length_aru32 > 0)
      {
          crc_u32 = crc_u32 ^ *(volatile U8 *)application_address_u32;
          application_address_u32 +=1;
        for (i = 8; --i >= 0;)
        {
            crc_u32 = (crc_u32 >> 1) ^ (0xedb88320 & -(crc_u32 & 1));
        }
        length_aru32--;
      }
      return ~crc_u32;
}

/**
=====================================================================================================================================================

@fn Name            : bootloader_app_uart_cmd_ack_send_v
@b Scope            : Private
@n@n@b Description  : Sends Bootloader Cmd through CAN0
@param Input Data   : command_aru8, command_status_aru8
@return Return Value: NULL

=====================================================================================================================================================
*/
void bootloader_app_uart_cmd_ack_send_v(U8 cmd,U8 cmd_ack_sts)
{

    U8 can_data[100] = {0,};
    U16 length = 0;
    can_data[0] = 0xDE;
    can_data[1] = 0xAD;
    can_data[2] = cmd;
    can_data[3] = (length+1) & 0xFF;		//length lo byte //status length is one more byte
    can_data[4] = (length+1) >> 8;		//length hi byte
    can_data[5] = cmd_ack_sts;
    can_data[length + 6] = 0xDE;
    can_data[length + 7] = 0xAD;   //TODO::implement CRC check
    can_data[length + 8] = 0x03;
    uart_tx_data(can_data, (length+9));

}

/**
=====================================================================================================================================================

@fn Name            : bootloader_app_uart_cmd_ack_send_v
@b Scope            : Private
@n@n@b Description  : Sends Bootloader Cmd through CAN0
@param Input Data   : command_aru8, command_status_aru8
@return Return Value: NULL

=====================================================================================================================================================
*/
void bootloader_app_uart_data_send_v(U8 cmd,U8 cmd_sts,U8 *data_au8,U16 length_u16)
{
    uint8_t can_data[100] = {0,};
    can_data[0] = 0xDE;
    can_data[1] = 0xAD;
    can_data[2] = cmd;
    can_data[3] = (length_u16+1) & 0xFF;		//length lo byte //status length is one more byte
    can_data[4] = (length_u16+1) >> 8;		//length hi byte
    can_data[5] = cmd_sts;
    if(length_u16 !=0)
    {
       memory_copy_u8_array_v(&can_data[6], data_au8, length_u16);
    }
    can_data[length_u16 + 6] = 0x00;
    can_data[length_u16 + 7] = 0x00;
    can_data[length_u16 + 8] = 0x03;
    uart_tx_data(can_data, (length_u16+9));
}




/**
=====================================================================================================================================================

@fn Name            : boot_loader_app_check_new_firmware_e
@b Scope            : Public
@n@n@b Description  : Check New Firmware in external flash memory
@param Input Data   : NULL
@return Return Value: boot_loader_status_te - zero if success, non-zero if failed

=====================================================================================================================================================
*/

bootloader_app_sts_te boot_loader_app_check_new_firmware_e(void)
{
	   bootloader_app_sts_te boot_loader_status_e = BOOT_LOADER_STATUS_UNKNOWN;
#if 0
    U32 delay_counter_u32 = 0;

    if(internal_flash_app_read_e(BOOTLOADER_CONFIG_START_ADDRESS, (U8*)&boot_loader_conf_gst, sizeof(boot_loader_conf_gst)) == IN_FLASH_SUCCESS)
    {
        if (0xAA == boot_loader_conf_gst.new_firmware_status_u8)
        {
            bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_SUCCESS_STS);
        	/**** Delay need for CAN tx complete incase no new firmware not in Flash Memory***/
            for (delay_counter_u32 = 0; delay_counter_u32 < 10000; delay_counter_u32++);
            boot_loader_status_e = BOOT_LOADER_UPDATE_NEW_FIRMWARE;
        }
        else
        {
            boot_loader_status_e = BOOT_LOADER_RUN_OLD_FIRMWARE;
            memory_set_u8_array_v((U8*)&boot_loader_conf_gst, 0, sizeof(boot_loader_conf_gst));
        }
    }
#endif
    return boot_loader_status_e;
}


/**
=====================================================================================================================================================

@fn Name            : boot_loader_app_jumping_to_main_application_e
@b Scope            : Public
@n@n@b Description  : Jumping from Bootloader to Main Application
@param Input Data   : NULL
@return Return Value: boot_loader_status_te - zero if success, non-zero if failed

=====================================================================================================================================================
*/


bootloader_app_sts_te boot_loader_app_jumping_to_main_application_e(void)
{
	bootloader_app_sts_te boot_loader_status_e = BOOT_LOADER_STATUS_UNKNOWN;
	 if (0x20000000 == ((*(volatile U32*) MAIN_APPLICATION_START_ADDRESS ) & 0x2FFE0000))
	 {
	     if (0xAA == boot_loader_conf_gst.new_firmware_status_u8)
	     {
	         //TODO:Erase Boot Config Message
	     	//eeprom_app_erase(0, 0);//erasing bootloader config data
	    	 in_flash_app_erase_e(BOOTLOADER_CONFIG_START_ADDRESS, BOOTLOADER_CONFIG_START_ADDRESS);
	    	 SystemSoftwareReset();
	     }
	    INT_SYS_DisableIRQGlobal();
	    // timer_deinit();
	     //rr_can_deinit_u8(CAN_0_INSTANCE);
	     //rr_can_deinit_u8(CAN_1_INSTANCE);
	    //SysTickIntDisable();
	    //SysTickDisable();
	    UART_Deinit(&uart_pal1_instance);
	    TIMING_Deinit(&timing_pal1_instance);
	     app_entry_u32 = *((U32*)MAIN_APPLICATION_START_ADDRESS);
	     JumpAddress =  *((U32*)(MAIN_APPLICATION_START_ADDRESS + 4));
	     JumpToApplication = (pFunction_1) JumpAddress;
	     /* Initialize user application's Stack Pointer */
	    // __set_MSP(app_entry_u32);
	     /* Jump to application */
	     JumpToApplication();
	     boot_loader_status_e = BOOT_LOADDER_JUMP_FAILURE;
	 }
	 else
	 {
	     boot_loader_status_e = BOOT_LOADER_N0_APPLICATION;
	 }
	 return boot_loader_status_e;
    //U32 delay_counter_u32 = 0;
 return boot_loader_status_e;
}




bootloader_app_sts_te boot_loader_app_jumping_to_Bootloader_section_e(void)
{
 bootloader_app_sts_te boot_loader_status_e = BOOT_LOADER_STATUS_UNKNOWN;
 if (0x20000000 == ((*(volatile U32*) BOOTLOADER_FIRMWARE_START_ADDRESS ) & 0x2FFE0000))
 {
     if (0xAA == boot_loader_conf_gst.new_firmware_status_u8)
     {
         //TODO:Erase Boot Config Message
     	//eeprom_app_erase(0, 0);//erasing bootloader config data
     }
     nvic_disable_all_Interrupts_v();
    //SysTickIntDisable();
    //SysTickDisable();
     app_entry_u32 = *((U32*)BOOTLOADER_FIRMWARE_START_ADDRESS);
     JumpAddress =  *((U32*)(BOOTLOADER_FIRMWARE_START_ADDRESS + 4));
     JumpToApplication = (pFunction_1) JumpAddress;
     /* Initialize user application's Stack Pointer */
    // __set_MSP(app_entry_u32);
     /* Jump to application */
     JumpToApplication();
     boot_loader_status_e = BOOT_LOADDER_JUMP_FAILURE;
 }
 else
 {
     boot_loader_status_e = BOOT_LOADER_N0_APPLICATION;
 }
 return boot_loader_status_e;
}


/**
=====================================================================================================================================================

@fn Name            : boot_loader_update_new_firmware
@b Scope            : Public
@n@n@b Description  : get new firmware via CAN and Directly Write in internal Flash
@param Input Data   : NULL
@return Return Value: NULL

=====================================================================================================================================================
*/
void bootloader_update_new_firmware(void)
{
#if 0
	bootlaoder_erase_application_section();
	bootloader_write_new_firmware();
	boot_loader_app_jumping_to_main_application_e();
#endif
#if 0
	if(1 == bootloader_cmd_received_u8)
	{
		bootloader_cmd_received_u8 = 0;
		bootloader_UART_rx_process(uart_bootloader_state_ge);
   }
#endif
	//uart_rx_data_queue();
#if 0
    while(1)
    {
        if(1 == can_app_received_msg_vu8)
        {
            can_app_received_msg_vu8 = 0;
			memory_copy_u8_array_v((U8*)&bootloader_can_rx_mst, (U8*)&can_app_0_rx_msg_index_gst, sizeof(can_app_0_rx_msg_index_gst));
            bootloader_can_rx_process(bootloader_can_rx_mst);
         }
        /*** After 10 Seconds No data Received From VIU ,ECU Send TIMEOUT CAN message to VIU  ***/
         if(bootloader_processing_timer_u32 > 10)
          {
             bootloading_process_enabled_u8 = 0;
             bootloader_processing_timer_u32 = 0;
             data_reset_count_u32 = 0;
             boot_loader_data_rec_count_u32 = 0;
             total_boot_loader_data_received_u32 = 0;
             bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID, BOOTLOADER_TIMEOUT_ERROR_WITH_DIAG_TOOL, BAPP_CAN_FAILURE_STS);
             can_bootloader_cmd_received_gvlu8 = 1;
          }
          if(1 == can_bootloader_cmd_received_gvlu8)
          {
             if(1 == one_second_timer_elapsed_gvu8)
             {
            	 one_second_timer_elapsed_gvu8 = 0;
            	 bootloader_processing_timer_u32 = 0;
            	 bootloading_process_enabled_u8 = 0;
                 bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID, BOOTLOADER_APPLICATION_NOT_AVAILABLE_STATUS, BAPP_CAN_FAILURE_STS);
              }
          }
         else
         {
        	 if(bootloading_process_enabled_u8 ==1)
        	 {
        		 if(1 == one_second_timer_elapsed_gvu8)
        		 {
        			 bootloader_processing_timer_u32++;
        		 }

        	 }
         }
    }
#endif
}

/**
=====================================================================================================================================================

@fn Name            : boot_loader_app_run_old_firmware_e
@b Scope            : Public
@n@n@b Description  : Run Old Firmware if available
@param Input Data   : NULL
@return Return Value: boot_loader_status_te - zero if success, non-zero if faileds

=====================================================================================================================================================
*/

bootloader_app_sts_te boot_loader_app_run_old_firmware_e(void)
{
    bootloader_app_sts_te boot_loader_status_e = BOOT_LOADER_STATUS_UNKNOWN;
    boot_loader_app_jumping_to_main_application_e();
    boot_loader_status_e = BOOT_LOADDER_JUMP_FAILURE;
    return boot_loader_status_e;
}


/**
=====================================================================================================================================================

@fn Name            : boot_loader_app_failure_v
@b Scope            : Public
@n@n@b Description  : bootloader app failure handling
@param Input Data   : NULL
@return Return Value: NULL

=====================================================================================================================================================
*/

void bootloader_app_failure_v(void)
{
	U8 send_counter_u8 = 0;
	bootloader_failed_vu8 = 1;
	while(1)
	{
#if 0
		if(1 == bootloader_cmd_received_u8)
		{
			bootloader_cmd_received_u8 = 0;
			bootloader_UART_rx_process(uart_bootloader_state_ge);
		}
		if((1 == timer_one_seconds_eplased_vu8) && (bootloader_failed_vu8 == 1))
		{
			timer_one_seconds_eplased_vu8 = 0;
			send_counter_u8++;
			if(send_counter_u8 >=25)
			{
				send_counter_u8 = 27;
				bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_APPLICATION_NOT_AVAILABLE_STATUS, BAPP_UART_SUCCESS_STS);
			}
		}
#endif
		uart_rx_data_queue();
	}

#if 0
    while(1)
    {
        if(1 == can_bootloader_cmd_received_gvlu8)
        {
            if(1== one_second_timer_elapsed_gvu8)
            {
            	//gpio_app_set_digital_output_v(S_LED1_RED_PTE9, state_e);
            	gpio_app_toggle_direct_output_v(S_LED1_RED_PTE9);
            	one_second_timer_elapsed_gvu8 = 0;
            	bootloader_processing_timer_u32 = 0;
            	bootloading_process_enabled_u8 = 0;
            	//bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID, BOOTLOADER_APPLICATION_NOT_AVAILABLE_STATUS, BAPP_CAN_FAILURE_STS);
            }
        }
        else
        {
        	if(bootloading_process_enabled_u8 == 1)
        	{
        		 if(1== one_second_timer_elapsed_gvu8)
        		 {
        			 bootloader_processing_timer_u32++;
        		 }
        	}
        }

          if(1 == can_app_received_msg_vu8)
          {
              can_app_received_msg_vu8 = 0;
              memory_copy_u8_array_v((U8*)&bootloader_can_rx_mst, (U8*)&can_app_0_rx_msg_index_gst, sizeof(can_app_0_rx_msg_index_gst));
              bootloader_can_rx_process(bootloader_can_rx_mst);
          }
          /*** After 10 Seconds No data Received From VIU ,ECU Send TIMEOUT CAN message to VIU  ***/
          if(bootloader_processing_timer_u32 >10)
          {
              bootloading_process_enabled_u8 = 0;
              bootloader_processing_timer_u32 = 0;
              data_reset_count_u32 = 0;
              boot_loader_data_rec_count_u32 = 0;
              total_boot_loader_data_received_u32 = 0;
              bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID, BOOTLOADER_TIMEOUT_ERROR_WITH_DIAG_TOOL, BAPP_CAN_FAILURE_STS);
              can_bootloader_cmd_received_gvlu8 = 1;
          }



    }
#endif

}
/**
=====================================================================================================================================================

@fn Name            : bootloader_internal_application_flash_erase_u8
@b Scope            : Public
@n@n@b Description  : Processes the erasse application code
@param Input Data   : NUll
@return Return Value: NULL

=====================================================================================================================================================
*/

void bootloader_internal_application_flash_erase_u8(void)
{
	/***** Todo:write application erase***/
   // if(!int_flash_app_erase(0, 1, 5))//application code start with 1 sector to 5 Sector
    if(1)
	{
    	//bootloader_app_can_ACK_cmd_send_v(BAPP_CAN_APP_ERASE_EN_PGN,BAPP_CAN_SUCCESS_STS,BAPP_CAN_CMD_SUCCESS_STS);
    }
    else
    {
    	//failure ack_can_msg
    	//bootloader_app_can_ACK_cmd_send_v(BAPP_CAN_ERROR_PGN,BAPP_CAN_FAILURE_STS,BAPP_WHOLE_APP_ERASE_FAIL);
    }
}
/**
=====================================================================================================================================================

@fn Name            : boot_loader_can_rx_task_v
@b Scope            : Public
@n@n@b Description  : Processes the can received msg
@param Input Data   : pgn_aru8 (PGN of received msg), data_length_aru8 (length of CAN msg)
@return Return Value: NULL

=====================================================================================================================================================
*/
void bootloader_UART_rx_process(bootloader_uart_app_list_te bootloader_state)
{
#if 0

	switch (bootloader_state)
	{
/**************************************************************************************************/
		case BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS:
		{
			bootloader_failed_vu8 = 0;
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_CONFIG_DATA_CMD_ACK_STATUS:
		{
			bootloader_enable_cmd_procees();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS:
		{
			bootloader_new_firmware_receive();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_END_OF_DATA_ACK_STATUS:
		{
			bootloader_end_of_data_handling();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_WHOLE_BIN_CRC_CHECK_STATUS:
		{
			bootloader_whole_crc_check();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS:
		{
			bootloader_jumping_to_boot_section();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_INT_APP_ERASE_STATUS:
		{
			bootlaoder_erase_application_section();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_INT_APP_WRITE_STATUS:
		{
			bootloader_write_new_firmware();
		}
		break;
/***************************************************************************************************/
		case BOOTLOADER_JUMPING_APPLICATION_STATUS:
		{
			boot_loader_app_jumping_to_main_application_e();
		}
		break;
/***************************************************************************************************/
		default:
		{

		}
		break;
/****************************************************************************************************/

	}
#endif
}





void bootloader_write_new_firmware(void)
{
#if 0
	 backup_app_start_adrs_u32 = NEW_APPLICATION_BACKUP_START_ADDRESS;
	 firmware_write_adrs_u32  = MAIN_APPLICATION_START_ADDRESS;
	 firmware_write_count_u32 = boot_loader_conf_gst.new_firmware_size_u32;
	 app_crc_u32 =0;

	while(firmware_write_count_u32>0)
	{
		if(firmware_write_count_u32 >=1024)
		{
			if(internal_flash_app_read_e(backup_app_start_adrs_u32, read_boot_loader_buffer_au8, INTERNAL_FLASH_BLOCK_SIZE) == IN_FLASH_SUCCESS)
			{
				if(internal_flash_app_write_e(firmware_write_adrs_u32, read_boot_loader_buffer_au8, INTERNAL_FLASH_BLOCK_SIZE) == IN_FLASH_SUCCESS)
				{
					backup_app_start_adrs_u32 += INTERNAL_FLASH_BLOCK_SIZE;
					firmware_write_adrs_u32 += INTERNAL_FLASH_BLOCK_SIZE;
					firmware_write_count_u32 -= INTERNAL_FLASH_BLOCK_SIZE;
					//system_mcu_wait_ms_v(50);
				}
				else
				{

					bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_FAILURE_STS);
					break;
				}
			}
			else
			{
				bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_FAILURE_STS);
				break;
			}

		}
		else
		{
			if(internal_flash_app_read_e(backup_app_start_adrs_u32, read_boot_loader_buffer_au8, firmware_write_count_u32) == IN_FLASH_SUCCESS)
			{
				if(internal_flash_app_write_e(firmware_write_adrs_u32, read_boot_loader_buffer_au8, firmware_write_count_u32) == IN_FLASH_SUCCESS)
				{
					backup_app_start_adrs_u32 += firmware_write_count_u32;
					firmware_write_adrs_u32 += firmware_write_count_u32;
					firmware_write_count_u32 -= firmware_write_count_u32;
				}
				else
				{
					bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_FAILURE_STS);
				}
			}
			else
			{
				bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_FAILURE_STS);
			}

		 }
	}
	if(firmware_write_count_u32 == 0)
	{
		app_crc_u32 = bootloader_app_whole_bin_crc32_generate_u32(MAIN_APPLICATION_START_ADDRESS,boot_loader_conf_gst.new_firmware_size_u32);

		if(app_crc_u32 != boot_loader_conf_gst.new_firmware_crc_u32)
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_SUCCESS_STS);
		}
		else
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_WRITE_STATUS, BAPP_UART_FAILURE_STS);
		}
	}
#endif

}



 void bootloader_enable_cmd_procees(void)
{

	//U32 big_endian_data_u32 = 0;
	received_boot_config_st.new_firmware_status_u8 = uart_rx_data.payload_au8[0];
	received_boot_config_st.new_firmware_size_u32 = (uart_rx_data.payload_au8[1] +
			(uart_rx_data.payload_au8[2] << 8) + (uart_rx_data.payload_au8[3] << 16) + (uart_rx_data.payload_au8[4] << 24));
	start_boot_loader_write_address_u32 = NEW_APPLICATION_BACKUP_START_ADDRESS;
	start_boot_loader_read_address_u32 = NEW_APPLICATION_BACKUP_START_ADDRESS;
     INT_SYS_DisableIRQGlobal();
	if(internal_flash_erase_e(NEW_APPLICATION_BACKUP_START_ADDRESS, NEW_APPLICATION_BACKUP_END_ADDRESS) == IN_FLASH_SUCCESS)
	{
		INT_SYS_EnableIRQGlobal();
		boot_loader_data_rec_count_u32 = 0;
		total_boot_loader_data_received_u32 = 0;

	     bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_CONFIG_DATA_CMD_ACK_STATUS, BAPP_UART_SUCCESS_STS);
	}
	else
	{
		bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_CONFIG_DATA_CMD_ACK_STATUS, BAPP_UART_FAILURE_STS);
	}

}



 void bootloader_new_firmware_receive(void)
{
#if 0
	U8 crc_check_data_au8[4];
	memory_copy_u8_array_v(&boot_loader_flash_data_au8[boot_loader_data_rec_count_u32],
			&uart_rx_data.payload_au8[1], (uart_rx_data.length_u16 - 1)); /* length+1 required? */
	boot_loader_data_rec_count_u32 += (uart_rx_data.length_u16 -1);
	total_boot_loader_data_received_u32 += (uart_rx_data.length_u16 -1);
	if (boot_loader_data_rec_count_u32 >= 1024)
	{
		boot_loader_data_rec_count_u32 = 0;
		if(internal_flash_app_write_e(start_boot_loader_write_address_u32,
				boot_loader_flash_data_au8,
				BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE) == IN_FLASH_SUCCESS)
		{

				if(internal_flash_app_read_e(start_boot_loader_read_address_u32,
											read_boot_loader_buffer_au8,
											BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE) == IN_FLASH_SUCCESS)
				{
	                if(!memory_comp_u8(read_boot_loader_buffer_au8, boot_loader_flash_data_au8, BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE))
	                {

	                   crc_check_sum_u32 = bootloader_app_crc32_generate_u32(read_boot_loader_buffer_au8,BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE);
	                   crc_check_data_au8[3] = crc_check_sum_u32 >> 24;
	                   crc_check_data_au8[2] = crc_check_sum_u32 >> 16;
	                   crc_check_data_au8[1] = crc_check_sum_u32 >> 8;
	                   crc_check_data_au8[0] = crc_check_sum_u32;
	                   bootloader_app_uart_data_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_SUCCESS_STS,crc_check_data_au8, 4);
	                   //bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_SUCCESS_STS);
	                   start_boot_loader_write_address_u32 += BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE;
	                   start_boot_loader_read_address_u32  += BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE;

	               }
	               else
	               {
	            	   bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_FAILURE_STS);
	                }
			   }
			   else
			   {
				   bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_FAILURE_STS);
			   }
		}
		else
		{

			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_FAILURE_STS);
		}
	}

#endif
}



 void bootloader_end_of_data_handling(void)
{
#if 0
	U8 crc_check_data_au8[4];
	if(boot_loader_data_rec_count_u32 == 0)
	{
		if(received_boot_config_st.new_firmware_size_u32 == total_boot_loader_data_received_u32)
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_SUCCESS_STS);
		}
		else
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_FAILURE_STS);
			total_boot_loader_data_received_u32 = 0;
		}
	}
	else
	{

		if(internal_flash_app_write_e(start_boot_loader_write_address_u32,
									  boot_loader_flash_data_au8, boot_loader_data_rec_count_u32) == IN_FLASH_SUCCESS)
		{
			if(internal_flash_app_read_e(start_boot_loader_read_address_u32,
										 read_boot_loader_buffer_au8, boot_loader_data_rec_count_u32) == IN_FLASH_SUCCESS)
			{
				if(!memory_comp_u8(read_boot_loader_buffer_au8, boot_loader_flash_data_au8, boot_loader_data_rec_count_u32))
				{


					 crc_check_sum_u32 = bootloader_app_crc32_generate_u32(read_boot_loader_buffer_au8,boot_loader_data_rec_count_u32);
					 crc_check_data_au8[3] = crc_check_sum_u32 >> 24;
					 crc_check_data_au8[2] = crc_check_sum_u32 >> 16;
					 crc_check_data_au8[1] = crc_check_sum_u32 >> 8;
				     crc_check_data_au8[0] = crc_check_sum_u32;
				     bootloader_app_uart_data_send_v(BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS, BAPP_UART_SUCCESS_STS,crc_check_data_au8, 4);
					 start_boot_loader_write_address_u32 += boot_loader_data_rec_count_u32;
					 start_boot_loader_read_address_u32  += boot_loader_data_rec_count_u32;
					 for(U32 delay_counter_u32 = 0;delay_counter_u32<50000;delay_counter_u32++);
					 if(received_boot_config_st.new_firmware_size_u32 == total_boot_loader_data_received_u32)
					 {
						 boot_loader_data_rec_count_u32 = 0;
						 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_SUCCESS_STS);

					 }
					 else
					 {
						 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_FAILURE_STS);
					     total_boot_loader_data_received_u32 = 0;
					     //int_flash_app_erase(1, 0, 1);//erase backup memory also Todo
					 }
				}
			}
			else
			{
				 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_FAILURE_STS);
			}
		}
		else
		{
			 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_END_OF_DATA_ACK_STATUS, BAPP_UART_FAILURE_STS);
		}
	}
#endif
}


 void bootloader_whole_crc_check(void)
{


	crc_check_sum_u32 = bootloader_app_whole_bin_crc32_generate_u32(NEW_APPLICATION_BACKUP_START_ADDRESS,
						  	  	  	  	  	  	  	  	  	  	  	 received_boot_config_st.new_firmware_size_u32);

	U32 received_crc_u32 = (uart_rx_data.payload_au8[1]+ (uart_rx_data.payload_au8[2] << 8) + (uart_rx_data.payload_au8[3] << 16) + (uart_rx_data.payload_au8[4] << 24));

	if(received_crc_u32 == crc_check_sum_u32)
	{
		bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_WHOLE_BIN_CRC_CHECK_STATUS, BAPP_UART_SUCCESS_STS);
		received_boot_config_st.new_firmware_crc_u32 =  received_crc_u32;
	}
	else
	{
		bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_WHOLE_BIN_CRC_CHECK_STATUS, BAPP_UART_SUCCESS_STS);
	}
}


void bootloader_jumping_to_boot_section(void)
{

	U8 write_data_au8[16];
	if(in_flash_app_erase_e(BOOTLOADER_CONFIG_START_ADDRESS, BOOTLOADER_CONFIG_END_ADDRESS) == IN_FLASH_SUCCESS)
	{
		memory_copy_u8_array_v((U8*)write_data_au8, (U8*)&received_boot_config_st, sizeof(received_boot_config_st));
		memory_copy_u8_array_v((U8*)&boot_loader_conf_gst,  (U8*)&received_boot_config_st, sizeof(received_boot_config_st));
		int_flash_write_data_u32 = (U32)&write_data_au8;
		if(internal_flash_app_write_e(BOOTLOADER_CONFIG_START_ADDRESS, write_data_au8, 16) == IN_FLASH_SUCCESS)
		{
			if(internal_flash_app_read_e(BOOTLOADER_CONFIG_START_ADDRESS,
											(U8*)&flash_read_boot_config_st, sizeof(received_boot_config_st)) == IN_FLASH_SUCCESS)
			{
				if(!memory_comp_u8((U8*)&received_boot_config_st,
	        					  (U8*)&flash_read_boot_config_st,  sizeof(received_boot_config_st)))
				{
					//bootloader_app_can_ACK_cmd_send_v(TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID,
						//							  BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS,
							//	 					  BAPP_CAN_SUCCESS_STS);
					//boot_loader_app_jumping_to_Bootloader_section_e();
					SystemSoftwareReset();
				}
			}
			else
			{
				bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
			}
		}
		else
		{
			bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
		}
	}
	else
	{
		bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS, BAPP_UART_FAILURE_STS);
	}

}

static void  bootlaoder_erase_application_section(void)
{

	 /**** TODO: Application Address erase and Write ***/
    if(in_flash_app_erase_e(MAIN_APPLICATION_START_ADDRESS, MAIN_APPLICATION_END_ADDRESS) == IN_FLASH_SUCCESS)
	{

		 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_ERASE_STATUS, BAPP_UART_SUCCESS_STS);
	 }
	 else
	 {
		 bootloader_app_uart_cmd_ack_send_v(BOOTLOADER_INT_APP_ERASE_STATUS, BAPP_UART_FAILURE_STS);
	 }

}
/****************************************************************************************************************************************************
 * EOF
 ***************************************************************************************************************************************************/


