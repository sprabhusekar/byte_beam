
#ifndef _UART_QUEUE_H_
#define _UART_QUEUE_H

#include "uart_pal1.h"
#include "can.h"

#define UART_QUEUE_SIZE (300)
#define SUCCESS 0
#define FAILED 1

typedef struct uart_queue_struct{
	uint32_t tail;
	uint32_t head;
	uint32_t full_status;
	uint32_t empty_status;
	uint8_t message[UART_QUEUE_SIZE];
}uart_queue_struct;

int32_t uart_rx_array_queue_put(volatile uint8_t queue_data_in);
int32_t uart_rx_array_queue_get(volatile uint8_t *queue_data_out);
uint32_t uart_rx_array_queue_size();


#endif
