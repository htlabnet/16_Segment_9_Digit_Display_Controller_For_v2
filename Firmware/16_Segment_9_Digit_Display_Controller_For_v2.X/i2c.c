/*
 * File:   i2c.c
 * Author: thotgamma
 *
 * Created on 2017/12/28, 20:27
 */

#include "i2c.h"

uint8_t i2c_buff[16];
uint8_t i2c_count;
uint8_t i2c_itr;

uint8_t i2c_targetAddr;
uint8_t i2c_dataAddr;

enum I2C_Module_Status_t {
    IDLE,
    WRITE_START,
    WRITE_TARGET,
    WRITE_ADDR,
    WRITE_CONTENT,
    WRITE_STOP,
    WRITE_CHECKSTOP,
    
    READ_START,
    READ_SENDTARGET,
    READ_SENDADDR,
    READ_SENDSTOP,
    READ_RESTART,
    READ_READTARGET,
    READ_READENABLE,
    READ_CONTENT,
    READ_ACK,
    READ_NACK,
    READ_STOP,
    READ_CHECKSTOP,
    READ_DONE
} I2C_Module_Status;

void I2C_Init() {
    TRISB0=1;		/* Set up I2C lines by setting as input */
    TRISB1=1;
    SSPSTAT=0x80;		/* Slew rate disabled, other bits are cleared */
    SSPCON1=0x28;	/* Enable SSP port for I2C Master mode, clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPCON2=0;
    SSPADD=49;
}

bool I2C_isAvailable() {
    return (I2C_Module_Status == IDLE);
}

int I2C_WriteBuff(uint8_t targetAddr, uint8_t dataAddr, uint8_t* input, uint8_t count) {
    if (!I2C_isAvailable()) return -1; 
    i2c_targetAddr = targetAddr;
    i2c_dataAddr = dataAddr;
    i2c_count = count;
    i2c_itr = 0;
    memcpy(i2c_buff, input, count);
    I2C_Module_Status = WRITE_START;
    return 0;
}

int I2C_RequestRead(uint8_t targetAddr, uint8_t dataAddr, uint8_t count) {
    if (!I2C_isAvailable()) return -1; 
    i2c_targetAddr = targetAddr;
    i2c_dataAddr = dataAddr;
    i2c_count = count;
    i2c_itr = 0;
    I2C_Module_Status = READ_START;
    return 0;
}

bool I2C_isReadable() {
    return (I2C_Module_Status == READ_DONE);
}

uint8_t* I2C_ReadBuff() {
    if (I2C_Module_Status != READ_DONE) return -1;
    I2C_Module_Status = IDLE;
    return i2c_buff;
}


void I2C_Task() {
    switch (I2C_Module_Status) {
        case IDLE:
            break;
            
        case WRITE_START:
            SSPCON2bits.SEN = 1;
            I2C_Module_Status = WRITE_TARGET;
            break;
            
        case WRITE_TARGET:
            if (SSPCON2bits.SEN) return;
            SSPBUF = i2c_targetAddr;
            I2C_Module_Status = WRITE_ADDR;
            break;
            
        case WRITE_ADDR:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            SSPBUF = i2c_dataAddr;
            I2C_Module_Status = WRITE_CONTENT;
            break;
            
        case WRITE_CONTENT:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            SSPBUF = i2c_buff[i2c_itr++];
            if (i2c_itr == i2c_count) I2C_Module_Status = WRITE_STOP;
            break;
            
        case WRITE_STOP:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            PEN = 1;
            I2C_Module_Status = WRITE_CHECKSTOP;
            break;
            
        case WRITE_CHECKSTOP:
            if (PEN) return;
            I2C_Module_Status = IDLE;
            break;
        // --------------------------------------- :: READ :: ------
        case READ_START:
            SSPCON2bits.SEN = 1;
            I2C_Module_Status = READ_SENDTARGET;
            break;
            
        case READ_SENDTARGET:
            if (SSPCON2bits.SEN) return;
            SSPBUF = i2c_targetAddr;
            I2C_Module_Status = READ_SENDADDR;
            break;
            
        case READ_SENDADDR:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            SSPBUF = i2c_dataAddr;
            I2C_Module_Status = READ_SENDSTOP;
            break;
            
        case READ_SENDSTOP:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            PEN = 1;
            I2C_Module_Status = READ_RESTART;
            break;
            
        case READ_RESTART:
            if (PEN) return;
            SSPCON2bits.SEN = 1;
            I2C_Module_Status = READ_READTARGET;
            break;
            
        case READ_READTARGET:
            if (SSPCON2bits.SEN) return;
            SSPBUF = (i2c_targetAddr | 0x01);
            I2C_Module_Status = READ_READENABLE;
            break;
            
        case READ_READENABLE:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            RCEN = 1;
            I2C_Module_Status = READ_CONTENT;
            break;
            
        case READ_CONTENT:
            if (!SSPSTATbits.BF) return;
            i2c_buff[i2c_itr++] = SSPBUF;
            if (i2c_itr == i2c_count) {
                ACKDT = 1;		/* Acknowledge data 1:NACK,0:ACK */
                I2C_Module_Status = READ_NACK;
            } else {
                ACKDT=0;		/* Acknowledge data 1:NACK,0:ACK */
                I2C_Module_Status = READ_ACK;
            }
            ACKEN=1;		/* Enable ACK to send */
            break;
            
        case READ_ACK:
            if (ACKEN) return;
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            RCEN = 1;
            I2C_Module_Status = READ_CONTENT;
            break;
            
        case READ_NACK:
            if (ACKEN) return;
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            I2C_Module_Status = READ_STOP;
            break;
            
        case READ_STOP:
            if (SSPSTATbits.BF || (SSPSTATbits.R_nW)) return;
            PEN = 1;
            I2C_Module_Status = READ_CHECKSTOP;
            break;
            
        case READ_CHECKSTOP:
            if (PEN) return;
            SSPIF = 0;
            I2C_Module_Status = READ_DONE;
            break;
            
        case READ_DONE:
            break;
    }
}
