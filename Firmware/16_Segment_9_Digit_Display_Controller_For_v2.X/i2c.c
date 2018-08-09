/*
 * File:   i2c.c
 * Author: thotgamma
 *
 * Created on 2017/12/28, 20:27
 */

#include "i2c.h"

void I2C_Init() {
    TRISB0=1;		/* Set up I2C lines by setting as input */
    TRISB1=1;
    SSPSTAT=0x80;		/* Slew rate disabled, other bits are cleared */
    SSPCON1=0x28;	/* Enable SSP port for I2C Master mode,
			clock = FOSC / (4 * (SSPADD+1))*/ 
    SSPCON2=0;
    SSPADD=49;
    SSPIE=1;		/* Enable SSPIF interrupt */
    SSPIF=0;
}

char I2C_Start(char slave_write_address) {
    SSPCON2bits.SEN=1;		/* Send start pulse */
    while(SSPCON2bits.SEN);	/* Wait for completion of start pulse */
    SSPIF=0;
    if(!SSPSTATbits.S)		/* Check whether START detected last */
    return 0;			/* Return 0 to indicate start failed */   
    return (I2C_Write(slave_write_address));	/* Write slave device address
						with write to communicate */
}

void I2C_Ready() {
    while(BCLIF);	/* Wait if bit collision interrupt flag is set*/

    /* Wait for Buffer full and read write flag*/
    while(SSPSTATbits.BF || (SSPSTATbits.R_nW));
    SSPIF=0;  		/* Clear SSPIF interrupt flag*/
}

char I2C_Write(unsigned char data) {
      SSPBUF=data;	/* Write data to SSPBUF*/
      I2C_Ready();
      if (ACKSTAT)	/* Check for acknowledge bit*/
        return 1;
      else
        return 2;
}

char I2C_Stop() {
    I2C_Ready();
    PEN=1;		/* Stop communication*/
    while(PEN);		/* Wait for end of stop pulse*/
    SSPIF = 0;
    if (!SSPSTATbits.P);/* Check whether STOP is detected last */
    return 0;		/* If not return 0 to indicate start failed*/
}

/* Read data from location and send ack or nack depending upon parameter*/

char I2C_Read(char flag) {
        int buffer=0;
        RCEN=1;			/* Enable receive */

	/* Wait for buffer full flag which when complete byte received */
        while(!SSPSTATbits.BF);
        buffer=SSPBUF;		/* Copy SSPBUF to buffer */

	/* Send acknowledgment or negative acknowledgment after read to 
	continue or stop reading */
        if(flag==0)
            I2C_Ack();
        else
            I2C_Nack();
        I2C_Ready();
        return(buffer);
}

void I2C_Ack() {
    ACKDT=0;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
    while(ACKEN);
 }

void I2C_Nack() {
    ACKDT=1;		/* Acknowledge data 1:NACK,0:ACK */
    ACKEN=1;		/* Enable ACK to send */
    while(ACKEN);
}