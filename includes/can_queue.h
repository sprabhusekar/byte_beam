/*
 * can_queue.h
 *
 *  Created on: 13-Jun-2022
 *      Author: bhaar
 */

#ifndef CAN_QUEUE_H_
#define CAN_QUEUE_H_
#include "can_pal.h"
#include "status.h"
#include "common_header.h"

#define CAN_QUEUE_SIZE 300
#define TRUE true
#define FALSE false

#define CAN_QUEUE_SUCCESS  STATUS_SUCCESS
#define CAN_QUEUE_FAILED  STATUS_ERROR

typedef struct {
	uint32_t cs;       /*!< Code and Status*/
    uint32_t id;       /*!< ID of the message */
    uint8_t data[8];  /*!< Data bytes of the CAN message*/
    uint8_t length;    /*!< Length of payload in bytes */
} can_q_message_t;

typedef struct can_queue_struct{
	uint32_t tail;
	uint32_t head;
	uint32_t full_status;
	uint32_t empty_status;
	can_q_message_t message[CAN_QUEUE_SIZE];
}can_queue_struct;




extern U8 can_rx_array_queue_put(volatile can_message_t queue_data_in);
extern U8 can_tx_array_queue_put(volatile can_message_t queue_data_in);
extern U32 can_rx_array_queue_get(volatile can_message_t *queue_data_out);
extern U32 can_tx_array_queue_size(void);
extern U32 can_rx_array_queue_size(void);

extern void can_queue_init(void);


#endif /* CAN_QUEUE_H_ */
