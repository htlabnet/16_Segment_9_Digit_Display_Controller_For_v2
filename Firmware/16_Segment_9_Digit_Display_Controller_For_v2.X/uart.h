/* 
 * File:   uart.h
 * Author: thotgamma
 *
 * Created on 2019/01/05, 21:47
 */

#ifndef UART_H
#define	UART_H
bool uartBuffIsWriteAble(void);
void writeUartBuff(uint8_t data);
void uart_task();

#endif	/* UART_H */

