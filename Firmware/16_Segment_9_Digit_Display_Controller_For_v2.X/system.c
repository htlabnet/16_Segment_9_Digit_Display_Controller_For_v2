/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

#include "system.h"

/** CONFIGURATION Bits **********************************************/
#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
//#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = ON
#pragma config BORV     = 3
#pragma config VREGEN   = ON      //USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
//#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//#pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
//#pragma config CP2      = OFF
//#pragma config CP3      = OFF
#pragma config CPB      = OFF
//#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
//#pragma config WRT2     = OFF
//#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
//#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
//#pragma config EBTR2    = OFF
//#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF

#pragma config FOSC  = HS       // 20MHz Xtal(分周なし)
#pragma config MCLRE = ON       // リセットピンを利用する
#pragma config LVP   = OFF      // 低電圧プログラミング機能使用しない(OFF)
#pragma config WDT   = OFF      // ウォッチドッグタイマーを利用しない

uint8_t digitPtr = 0; // 現在表示している桁数
bool    showDemoMessage = false;

uint8_t led_stat;     // コントローラについてる4つのbarLEDの点灯状態

//初期値"*********"
uint32_t segMap[9] = {
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111,
    0b110000000011111111
};

void refreshShiftRegister(int ptr) {
    uint16_t ledSelector = 0b1 << ptr;

    uint32_t map =    ((segMap[ptr] & 0b11111111) << 24)
                    | ((segMap[ptr] & 0b1111111100000000) << 8)
                    | ((ledSelector & 0b0000000011) << 14)
                    | ((led_stat    & 0b00001111) << 10)
                    | ((segMap[ptr] & 0b110000000000000000) >> 8)
                    | ((ledSelector & 0b1111111100) >> 2);

    for (int i = 0; i < 32; i++) {
        LATBbits.LATB2 = (map >> i) & 1;
        LATBbits.LATB3 = 1;
        __delay_us(1);
        LATBbits.LATB3 = 0;
        __delay_us(1);
    }

    LATBbits.LATB4 = 1;
    __delay_us(1);
    LATBbits.LATB4 = 0;
    __delay_us(1);
}

uint16_t divisor = 0;
uint8_t display[9];
uint8_t disdot[9];
uint16_t writeItr = 0;

void handleMessage() {
    if (divisor++ == 500) {
        divisor = 0;
        
        if (writeItr++ == MESSAGE_LENGTH-9) writeItr = 0;
        strncpy(display, DEMO_MESSAGE + writeItr, 9);
        strncpy(disdot,  DEMO_DOTFLAG + writeItr, 9);
        
        setMsgWithDot(display, disdot);
        
    }
}

uint8_t sel_before;
uint8_t up_before;
uint8_t down_before;

uint8_t year = 00;
uint8_t mon  = 00;
uint8_t date = 00;
uint8_t day  = 00;
uint8_t hour = 00;
uint8_t min  = 00;
uint8_t sec  = 00;

uint8_t rtcSetState = 0;

uint8_t buffer[10];

void writeRTC() {
    
    setMsg("WRITING  ");
    
    I2C_Init();
    I2C_Start(0xD0);
    I2C_Write(0x00);

    I2C_Write(dec2bcd(sec)); // Seconds
    I2C_Write(dec2bcd(min)); // Minutes
    I2C_Write(dec2bcd(hour)); // Hours
    I2C_Write(dec2bcd(day+1)); // Day
    I2C_Write(dec2bcd(date+1)); // Date
    I2C_Write(dec2bcd(mon+1)); // Month
    I2C_Write(dec2bcd(year)); // Year
    I2C_Stop();

    setMsg("DONE     ");
}

    int counter = 0;
//次の桁を表示する
void interrupt isr(void) {
    if (INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = 0;    // フラグを下げる
        TMR0 = 0xFF - 125+1;
        
        if (showDemoMessage) handleMessage();
        
        refreshShiftRegister(digitPtr);
        digitPtr = (digitPtr+1)%9;      // digitPtrを次の値にセット
    }
    
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif

    if (PORTDbits.RD7) { // 時刻合わせMODE

        switch (rtcSetState) {
            case 0: // year
                if ((BUTTON_UP != up_before) && !BUTTON_UP) year = (++year)%100;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) year = (--year)%100; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "YEAR-20%02d", year);
                setMsg(buffer);
                break;
            case 1: // mon
                if ((BUTTON_UP != up_before) && !BUTTON_UP) mon = (++mon)%12;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) mon = (--mon)%12; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "MON -  %2d", mon+1);
                setMsg(buffer);
                break;
            case 2: // date
                if ((BUTTON_UP != up_before) && !BUTTON_UP) date = (++date)%31;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) date = (--date)%31; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "DATE-  %2d", date+1);
                setMsg(buffer);
                break;
            case 3: // day
                if ((BUTTON_UP != up_before) && !BUTTON_UP) day = (++day)%7;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) day = (--day)%7; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "DAY -  %2d", day+1);
                setMsg(buffer);
                break;
            case 4: // hour
                if ((BUTTON_UP != up_before) && !BUTTON_UP) hour = (++hour)%24;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) hour = (--hour)%24; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "HOUR-  %2d", hour);
                setMsg(buffer);
                break;
            case 5: // min
                if ((BUTTON_UP != up_before) && !BUTTON_UP) min = (++min)%60;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) min = (--min)%60; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtcSetState++;
                sprintf(buffer, "MIN -  %2d", min);
                setMsg(buffer);
                break;
            case 6: // sec
                if ((BUTTON_UP != up_before) && !BUTTON_UP) sec = (++sec)%60;
                if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) sec = (--sec)%60; 
                if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) {
                    rtcSetState++;
                    writeRTC();
                    break;
                }
                sprintf(buffer, "SEC -  %2d", sec);
                setMsg(buffer);
                break;
            case 7: // done
                break;
            default:
                setMsg("ERROR!!!!");
                break;
        }

        up_before = BUTTON_UP;
        down_before = BUTTON_DOWN;
        sel_before = BUTTON_SEL;
        return;
    }
        
    if (PORTDbits.RD6 && counter++ == 100) {
        counter = 0;
        I2C_Start(0xD0);
        I2C_Write(0x00);
        I2C_Stop();

        I2C_Start(0xD1);

        uint8_t rtcdata[7];
        for (int i = 0; i < 7; i++) {
            rtcdata[i] = I2C_Read(i == 6);
        }

        I2C_Stop();


        uint8_t buffer[10];
        sprintf(buffer, "%02d%1d%02d%02d%02d", bcd2dec(rtcdata[4]), bcd2dec(rtcdata[3]), bcd2dec(rtcdata[2] & 0x3F), bcd2dec(rtcdata[1]), bcd2dec(rtcdata[0] & 0x7F));
        setMsg(buffer);

        //uint8_t buffer[10];
        //sprintf(buffer, "%09d", counter++);
        //setMsg(buffer);

    }
        
        
}