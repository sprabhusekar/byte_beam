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
extern uint8_t last_uart_command_ack_pending;
extern uint8_t last_uart_command_ack_pending;
U8 heart_beat_check(void);
extern U8 send_heartbeat(void);
U8 process_uart_data(U8 data);

extern  U8 last_uart_command_ack_pending;




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

typedef struct __attribute__((packed)) UART_data_struct
{
	U16 stx_U16;
	U8 cmd_u8;
	U16 length_u16;
	U8 payload_au8[100];
	U16 crc_u16;
	U64 timestamp_u64;
	U8 etx_u8;
} UART_data_struct;

#endif /* UART_STATE_MACHINE_H_ */
