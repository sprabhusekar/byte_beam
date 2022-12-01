#ifndef TORK_UPDATE_APP_H_
#define TORK_UPDATE_APP_H_

#include "common_header.h"
#include "can.h"

typedef enum tork_bldr_state_te_tag
{
	TORK_BLDR_INIT_ACK,
	TORK_BLDR_CODE_SIZE_CMD_ACK,
	TORK_BLDR_ERASE_CMD_ACK,
	TORK_BLDR_WRITE_DATA_BLOCK_ACK,
	TORK_BLDR_JMP_CMD_ACK,
}tork_bldr_state_te;

extern uint8_t deac_heart_beat;

int tork_update_app_process_can_msg(can_message_t message_can);
void tork_update_app_uart_cmd_send_v(U8 cmd,U8 ack_status);

#endif
