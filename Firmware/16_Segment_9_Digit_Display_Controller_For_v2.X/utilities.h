/* 
 * File:   utilities.h
 * Author: thotgamma
 *
 * Created on 2018/08/08, 18:13
 */

#ifndef UTILITIES_H
#define	UTILITIES_H

#include <stdint.h>

#include "system.h"
#include "segFonts.h"

void showBinary(uint8_t input);
void setMsg(uint8_t input[]);
void setMsgWithDot(uint8_t message[], uint8_t dotFlag[]);
uint8_t bcd2dec(uint8_t input);
uint8_t dec2bcd(uint8_t input);

#endif	/* UTILITIES_H */

