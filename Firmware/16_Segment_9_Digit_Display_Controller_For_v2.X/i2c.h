/* 
 * File:   i2c.h
 * Author: thotgamma
 *
 * Created on 2017/12/28, 20:27
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <xc.h>

void I2C_Init();
bool I2C_isAvailable();
bool I2C_isReadable();
int I2C_WriteBuff(uint8_t targetAddr, uint8_t dataAddr, uint8_t* input, uint8_t count);
int I2C_RequestRead(uint8_t targetAddr, uint8_t dataAddr, uint8_t count);
uint8_t* I2C_ReadBuff();
bool I2C_isReady();
void I2C_Task();

#endif	/* I2C_H */
