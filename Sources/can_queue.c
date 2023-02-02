/*
 * can_queue.c
 *
 *  Created on: 13-Jun-2022
 *      Author: bhaar
 */
#include "can_queue.h"
#include "malloc.h"
#include "string.h"
#include "common_header.h"



 static can_queue_struct rx_can_queue __attribute__((section("m_data"), used)) = { .tail = 0, .head = 0, .full_status = 0,
		.empty_status = true, .message[CAN_QUEUE_SIZE - 1] = { { 0 } }, } ;

// static can_queue_struct tx_can_queue __attribute__((section("m_data_2"), used)) = { .tail = 0, .head = 0, .full_status = 0,
//		.empty_status = true, .message[CAN_QUEUE_SIZE - 1] = { { 0 } }, } ;



//static can_queue_struct tx_can_queue = { .tail = 0, .head = 0, .full_status = 0,
//		.empty_status = TRUE, .message[CAN_QUEUE_SIZE - 1] = { { 0 } }, };

U8 can_rx_array_queue_put(volatile can_message_t queue_data_in)
{

	if (rx_can_queue.full_status == TRUE)
	{
		return CAN_QUEUE_FAILED; /* Queue Full*/
	}
	else
	{
		memcpy(&rx_can_queue.message[rx_can_queue.tail], &queue_data_in, sizeof(can_message_t));
		rx_can_queue.empty_status = FALSE;
	}

	if (rx_can_queue.tail == ((rx_can_queue.head - 1 + CAN_QUEUE_SIZE) % CAN_QUEUE_SIZE))
	{

		rx_can_queue.full_status = TRUE;
		return CAN_QUEUE_SUCCESS;
	}
	else
	{
		rx_can_queue.full_status = FALSE;
	}
	rx_can_queue.tail = (rx_can_queue.tail + 1) % CAN_QUEUE_SIZE;
	return CAN_QUEUE_SUCCESS; // No errors
}

//U8 can_tx_array_queue_put(volatile can_message_t queue_data_in)
//{
//
//	if (tx_can_queue.full_status == TRUE)
//	{
//		return CAN_QUEUE_FAILED; /* Queue Full*/
//	}
//	else
//	{
//		memcpy(&tx_can_queue.message[tx_can_queue.tail], &queue_data_in, sizeof(can_message_t));
//		tx_can_queue.empty_status = FALSE;
//	}
//
//	if (tx_can_queue.tail == ((tx_can_queue.head - 1 + CAN_QUEUE_SIZE) % CAN_QUEUE_SIZE))
//	{
//		tx_can_queue.full_status = TRUE;
//		return CAN_QUEUE_SUCCESS;
//	}
//	else
//	{
//		tx_can_queue.full_status = FALSE;
//	}
//
//	tx_can_queue.tail = (tx_can_queue.tail + 1) % CAN_QUEUE_SIZE;
//	return CAN_QUEUE_FAILED; // No errors
//}


U32 can_rx_array_queue_get(volatile can_message_t *queue_data_out)
{

	if (rx_can_queue.empty_status == TRUE)
	{
		return CAN_QUEUE_FAILED;
	}
	else
	{
		memcpy(queue_data_out, (U8*)&rx_can_queue.message[rx_can_queue.head], sizeof(can_message_t));
		rx_can_queue.full_status = FALSE;
	}
	rx_can_queue.head = (rx_can_queue.head + 1) % CAN_QUEUE_SIZE;
	if (rx_can_queue.tail == rx_can_queue.head)
	{
		rx_can_queue.empty_status = TRUE;
		rx_can_queue.full_status = FALSE;
		return CAN_QUEUE_SUCCESS;
	}
	else
	{
		rx_can_queue.empty_status = FALSE;
	}
	return 0; // No errors
}
//U32 can_tx_array_queue_get(volatile can_message_t *queue_data_out)
//{
//	if (tx_can_queue.empty_status == TRUE)
//	{
//		return CAN_QUEUE_FAILED;
//	}
//	else
//	{
//		memcpy(queue_data_out, &tx_can_queue.message[tx_can_queue.head], sizeof(can_message_t));
//		rx_can_queue.full_status = FALSE;
//	}
//	tx_can_queue.head = (tx_can_queue.head + 1) % CAN_QUEUE_SIZE;
//	if (tx_can_queue.tail == tx_can_queue.head)
//	{
//		tx_can_queue.empty_status = TRUE;
//		tx_can_queue.full_status = FALSE;
//		return CAN_QUEUE_SUCCESS;
//	}
//	else
//	{
//		tx_can_queue.empty_status = FALSE;
//	}
//	return 0; // No errors
//}

U32 can_rx_array_queue_size(void)
{
	if (rx_can_queue.head == rx_can_queue.tail)
	{
		return 0;
	}

	if (rx_can_queue.full_status == 1)
	{
		return CAN_QUEUE_SIZE;
	}

	return rx_can_queue.tail > rx_can_queue.head ?
			rx_can_queue.tail - rx_can_queue.head :
			CAN_QUEUE_SIZE - (rx_can_queue.head - rx_can_queue.tail);
}
//U32 can_tx_array_queue_size(void)
//{
//	if (tx_can_queue.head == tx_can_queue.tail)
//	{
//		return 0;
//	}
//
//	if (tx_can_queue.full_status == 1)
//	{
//		return CAN_QUEUE_SIZE;
//	}
//
//	return tx_can_queue.tail > tx_can_queue.head ?
//			rx_can_queue.tail - rx_can_queue.head :
//			CAN_QUEUE_SIZE - (rx_can_queue.head - rx_can_queue.tail);
//}


void can_queue_init(void)
{
	rx_can_queue.empty_status = 1;
	rx_can_queue.full_status = 0;
	rx_can_queue.head = 0;
	rx_can_queue.tail = 0;
//	tx_can_queue.empty_status = 1;
//	tx_can_queue.full_status = 0;
//	tx_can_queue.head = 0;
//	tx_can_queue.tail = 0;
}
