/*
 * File:   uart.c
 * Author: thotgamma
 *
 * Created on 2019/01/05, 21:47
 */


#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "uart.h"
#include "system.h"


const uint8_t uartBuffSize = 32;
uint8_t uartBuff[uartBuffSize];
uint8_t uartBuffWritePtr = 0;
uint8_t uartBuffReadPtr = 0;

bool uartBuffIsWriteAble(void) {
	if (uartBuffReadPtr == ((uartBuffWritePtr+1)%uartBuffSize)) return false;
	else return true;
}

bool uartBuffIsReadAble(void) {
	if (uartBuffReadPtr == uartBuffWritePtr) return false;
	else return true;
}

void writeUartBuff(uint8_t data) {
	if (!uartBuffIsWriteAble()) return;
	uartBuff[uartBuffWritePtr] = data;
	uartBuffWritePtr = (uartBuffWritePtr + 1) % uartBuffSize;
}

uint8_t readUartBuff(void) {
	if (!uartBuffIsReadAble()) return 0;
	uint8_t tmp = uartBuff[uartBuffReadPtr];
	uartBuffReadPtr = (uartBuffReadPtr + 1) % uartBuffSize;
	return tmp;
}


bool UART_ReadingNextValue = false;
uint8_t digitSelector;    // 書き換え桁数
uint32_t dotflag;  // ドットをつけるかどうか

void uart_task() {
    if (uartBuffIsReadAble()) {
        uint8_t RxData = readUartBuff();
        if (UART_ReadingNextValue == false) {
            if ((RxData & 0b11100000) == 0b11100000) {
                digitSelector = (RxData & 0b00001111);
                dotflag = (RxData & 0b00010000) >> 4;
                UART_ReadingNextValue = true;
            }
        } else {
            UART_ReadingNextValue = false;
            if (digitSelector > 8) return;  // 無効な入力の処理
            if (RxData > 0b01111111) RxData = ~RxData;
            segMap[digitSelector] = ~(fontList[RxData] | (dotflag << 16)); // 値を実際にセット
        }
    }
}