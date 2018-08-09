/* 
 * File:   i2c.h
 * Author: thotgamma
 *
 * Created on 2017/12/28, 20:27
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>
#include <xc.h>

void I2C_Init();

char I2C_Start(char slave_write_address);

void I2C_Ready();

char I2C_Write(unsigned char data);

char I2C_Stop();

char I2C_Read(char flag);

void I2C_Ack();

void I2C_Nack();

#endif	/* I2C_H */
