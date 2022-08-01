/*
 * uart.h
 *
 *  Created on: 11-Jun-2022
 *      Author: bhaar
 */

#ifndef UART_H_
#define UART_H_

uint8_t uart_init(void);
uint8_t uart_tx_data(uint8_t *tx_buffer_au8,uint32_t tx_size_u32);
uint32_t uart_rx_data_queue(void);

void SPLL_init_112MHz(void) ;
void SOSC_init_8MHz(void);
void HighSpeedRUNmode (void);

#endif /* UART_H_ */
