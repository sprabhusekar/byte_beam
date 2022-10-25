#ifndef APPLICATION_LAYER_INC_BOOTLOADER_APP_H_
#define APPLICATION_LAYER_INC_BOOTLOADER_APP_H_

/* Includes for definitions */
#include "common_header.h"

////#include "flash_drv.h"


/* Macro Definitions */

#define BOOTLOADER_FIRMWARE_START_ADDRESS						(U32)0x00000000
#define BOOTLOADER_FIRMWARE_END_ADDRESS							(U32)0x0000B000

#define BOOTLOADER_CONFIG_START_ADDRESS							(U32)0x0000C000
#define BOOTLOADER_CONFIG_END_ADDRESS							(U32)0x0000D000

#define MAIN_APPLICATION_START_ADDRESS							(U32)0x0000E000
#define MAIN_APPLICATION_END_ADDRESS							(U32)0x00042000

#define NEW_APPLICATION_BACKUP_START_ADDRESS					(U32)0x00043000
#define NEW_APPLICATION_BACKUP_END_ADDRESS						(U32)0x00076000

#define DATA_BACKUP_STORAGE_START_ADDRESS						(U32)0x00077000
#define DATA_BACKUP_STORAGE_END_ADDRESS							(U32)0x0007B000







#define POLY 						0x8408







#define DIAG_TOOL_CONNECTION_BOOT_CMD_DATA  	0x50
#define BOOTLOADER_INTERNAL_FLASH_WRITE_SIZE    1024
/** @{
 * Type definition */
/** Boot_Loader Status **/
typedef enum bootloader_app_sts_te_tag
{

    BOOT_LOADER_SUCCESS = 0,
    BOOT_LOADER_FAILURE,
    BOOT_LOADER_N0_APPLICATION,
    BOOT_LOADDER_JUMP_FAILURE,
    BOOT_LOADER_RUN_OLD_FIRMWARE,
    BOOT_LOADER_UPDATE_NEW_FIRMWARE,
    BOOT_LOADER_STATUS_UNKNOWN
} bootloader_app_sts_te;



typedef enum bootloader_can_rx_id_list_te_tag
{
	BOOTLOADER_RX_DIAG_TOOL_CNT_CMD_ID = 0x700,
	BOOTLOADER_RX_BOOT_CMD_ID=0x701,
	BOOTLOADER_RX_DATA_ID=0x702,
}bootloader_can_rx_id_list_te_tag;

typedef enum bootloader_stage_cmd_list_te_tag
{
	BOOTLOADER_TX_DIAG_BOOTLOADER_TOOL_CNT_CMD_ACK_ID = 0,
	BOOTLOADER_RX_ENABLE_BOOTLOADER_CMD_ID,
	BOOTLOADER_1KB_CRC_ACK_STATUS_CMD_ID,
	BOOTLOADER_RX_END_DATA_ID,
	BOOTLOADER_RX_WHOLE_CRC_CHECK_CMD_ID,
	BOOTLOADER_RX_CMD_JUMPING_TO_BOOTLOADER,
	BOOTLOADER_RX_APPLICATION_ERASE_CMD_ID,
	BOOTLOADER_RX_APPLICATION_NEW_FIRMWARE_WRITE_CMD_ID,
	BOOTLOADER_RX_CMD_JUMPING_TO_APPLICATION_SECTION_ID,
	BOOTLOADER_RX_CRC_ERROR_ID,

}bootloader_stage_cmd_list_te;

typedef enum bootloader_can_tx_id_list_te_tag
{
	TX_CMD_DIAG_BOOTLOADER_TOOL_CNT_ACK_ID= 0x710,

}bootloader_can_tx_id_list_te_tag;

typedef enum bootloader_app_btldr_can_frame_te_tag
{
    BAPP_CAN_CMD_FRME = 0xAA, /* BTLDR -BOOTLOADER */
    BAPP_CAN_ACK_FRME = 0x51, BAPP_CAN_ERROR_FRME = 0x53,
} bootloader_app_btldr_can_frame_te;

/* Enums for CMD_TYPE ACKS ENDS*/

typedef enum bootloader_app_btldr_can_sts_te_tag
{
    BAPP_UART_SUCCESS_STS = 0xAA, /* BTLDR -BOOTLOADER */
    BAPP_UART_FAILURE_STS = 0xFF /* SACK - Success ACK*/
} bootloader_app_btldr_can_sts_te;

#if 0
typedef enum bootloader_app_error_list_te_tag
{
	DIAG_TOOL_CONNT_CMD_ID_ACK_STATUS,
	BOOTLOADER_CMD_ENABLE_ACK_STATUS,
	BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS,
	BOOTLOADER_END_OF_DATA_ACK_STATUS,
	BOOTLOADER_WHOLE_BIN_CRC_CHECK_STATUS,
	BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS,
	BOOTLOADER_INT_APP_ERASE_STATUS,
	BOOTLOADER_INT_APP_WRITE_STATUS,
	BOOTLOADER_JUMPING_APPLICATION_STATUS,
	BOOTLOADER_APPLICATION_NOT_AVAILABLE_STATUS,
	BOOTLOADER_TIMEOUT_ERROR_WITH_DIAG_TOOL,

}bootloader_app_error_list_te;
#endif
typedef enum bootloader_uart_app_list_te_tag
{
	BOOTLOADER_MODE_ENABLE_CMD_ACK_STATUS = 5,
	BOOTLOADER_CONFIG_DATA_CMD_ACK_STATUS,
	BOOTLOADER_1KB_CRC_DATA_RECEIVED_ACK_STATUS,
	BOOTLOADER_END_OF_DATA_ACK_STATUS,
	BOOTLOADER_WHOLE_BIN_CRC_CHECK_STATUS,
	BOOTLOADER_JUMPING_TO_BOOTLOADER_STATUS,
	BOOTLOADER_INT_APP_ERASE_STATUS,
	BOOTLOADER_INT_APP_WRITE_STATUS,
	BOOTLOADER_JUMPING_APPLICATION_STATUS,
	BOOTLOADER_APPLICATION_NOT_AVAILABLE_STATUS,
	BOOTLOADER_TIMEOUT_ERROR_WITH_DIAG_TOOL,
	BOOTLOADER_STATE_UNKNOWN,
}bootloader_uart_app_list_te;

extern bootloader_uart_app_list_te uart_bootloader_state_ge;

typedef struct  __attribute__((packed))  bootloader_config_tst_tag
{

	U32 new_firmware_size_u32;
    U32 new_firmware_crc_u32;
    U32 dummy_byte_1_u32;
    U16 dummy_byte_3_u16;
    U8 new_firmware_status_u8;
    U8 diag_tool_public_key_u8;

   //for storing data in internal flash memory data should be in multiple of 8 byte Data

}bootloader_config_tst;


typedef struct  __attribute__((packed)) bootlaoder_diag_tool_ack_st_tag
{
	U8 ack_status_u8;
	U8 boot_vcu_public_key_u8;
	U16 vcu_firmware_version_no_u16;
}bootlaoder_diag_tool_ack_st;

typedef struct  __attribute__((packed))  bootlaoder_PGP_key_st_tag
{
	U8 bootloader_vcu_private_key_u8;
	U8 bootloader_vcu_public_key_u8;
	U8 bootloader_Diag_tool_public_key_u8;
}bootlaoder_PGP_key_st;
//typedef struct boot_loader_config_tst_tag ;
/** @} */
/** @{
 *  Public Variable Definitions */
extern U8 can_bootloader_cmd_received_gvlu8;
extern U8 bootloading_process_enabled_u8;
extern U32 bootloader_processing_timer_u32;
extern bootlaoder_PGP_key_st bootloader_diag_tool_pgp_key_st;

extern U8 boot_loader_flash_data_au8[10];
extern U8 read_boot_loader_buffer_au8[10];
extern U8 bootloader_checksum_byte_au8[8];

extern U32 total_boot_loader_data_received_u32;
extern U32 boot_loader_data_rec_count_u32;
extern U32 data_reset_count_u32;
extern U32 start_boot_loader_write_address_u32;
extern U32 start_read_boot_loader_address_u32;
extern U32 boot_loader_config_flash_start_address_u32;


//extern flash_drv_blocks_erase_tst boot_loader_block_erase_st;
//extern storage_task_update_boot_data_tst boot_loader_store_st;
extern bootloader_app_sts_te boot_loader_status_e;

extern U8 btldr_data_channel_source_u8;
extern U8 bootloader_app_os_comm_event_type_gu8;

/** @} */

/* Public Function Prototypes **/
//void bootloader_can_rx_process(can_app_msg_tst boot_can_rx_st);
U32 bootloader_app_crc32_generate_u32(U8 *data_arpu8, U32 length_aru32);
U32 bootloader_app_whole_bin_crc32_generate_u32(U32 application_address_u32,
                                                U32 length_aru32);
bootloader_app_sts_te boot_loader_app_check_new_firmware_e(void);
void bootloader_internal_application_flash_erase_u8(void);
void bootloader_app_failure_v(void);
void bootloader_update_new_firmware(void);
bootloader_app_sts_te boot_loader_app_run_old_firmware_e(void);
void updater_lld_stop(void);
void bootloader_jumping_to_boot_section(void);
void bootloader_write_new_firmware(void);
bootloader_app_sts_te boot_loader_app_jumping_to_Bootloader_section_e(void);
void bootloader_UART_rx_process(bootloader_uart_app_list_te bootloader_state);
void bootloader_app_uart_data_send_v(U8 cmd,U8 cmd_sts,U8 *data_au8,U16 length_u16);

void bootloader_app_uart_cmd_ack_send_v(U8 cmd,U8 cmd_ack_sts);

void bootloader_enable_cmd_procees(void);
void bootloader_new_firmware_receive(void);
void bootloader_whole_crc_check(void);
void bootloader_end_of_data_handling(void);
#endif /* APPLICATION_LAYER_BATTERY_MANAGEMENT_BOOTLOADER_APP_H_ */
