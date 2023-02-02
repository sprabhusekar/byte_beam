/*
 * uart_state_machine.h
 *
 *  Created on: 23-Jul-2022
 *      Author: Prabhu
 */

#ifndef UART_STATE_MACHINE_H_
#define UART_STATE_MACHINE_H_

#include "common_header.h"

extern volatile U16 new_rx_count_length_16;
//extern uint8_t last_uart_command_ack_pending;
extern uint8_t last_uart_command_ack_pending;
extern U8 heart_beat_received;

U8 heart_beat_check(void);
extern U8 send_heartbeat(void);
U8 process_uart_data(U8 data);
extern  U16 uart_receive_counter_u16;


//extern  U8 last_uart_command_ack_pending;

extern U8 bootloader_enable_command_received_u8;


typedef enum UART_PARSER_STATE_te_tag
{
	STX1_STATE = 0,
	STX2_STATE,
	CMD_STATE,
	LEN1_STATE,
	LEN2_STATE,
	DATA_STATE,
	CRC1_STATE,
	CRC2_STATE,
	TIMESTAMP_STATE,
	ETX_STATE,
	STATE_MAX
} UART_PARSER_STATE;


typedef enum UART_COMMAND_LIST_te_tag
{
   UART_HEART_BIT_ACK_COMMAND = 0x00,
   UART_CAN_ACK_COMMAND= 0x04,
   UART_BOOTLOADER_ENABLE_COMMAND,
   UART_BOOTLOADER_DATA_RECEIVED_COMMAND,
   UART_BOOTLOADER_STATUS_COMMAND,
   UART_MAX_COMMAND,
   UART_EXT_ECU_UPDATE_COMMAND = 0xAB,
} UART_COMMAND_LIST_te;

typedef struct __attribute__((packed)) UART_data_struct
{
	U16 stx_U16;
	U8 cmd_u8;
	U16 length_u16;
	U8 payload_au8[300];
	U16 crc_u16;
	U64 timestamp_u64;
	U8 etx_u8;
} UART_data_struct;

typedef struct UART_rx_canframe_data_struct_tag
{
	U8 can_payload[13];
} UART_rx_canframe_data_struct;

extern UART_data_struct uart_rx_data;

#endif /* UART_STATE_MACHINE_H_ */
