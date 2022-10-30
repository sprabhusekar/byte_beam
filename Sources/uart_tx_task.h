/*
 * uart_tx_task.h
 *
 *  Created on: 01-Aug-2022
 *      Author: Prabhu
 */

#ifndef UART_TX_TASK_H_
#define UART_TX_TASK_H_

extern  void uart_tx_task_v(void *parameter);

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

#endif /* UART_TX_TASK_H_ */
