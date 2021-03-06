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
    if (divisor++ == 1500) {
        divisor = 0;
        
        if (writeItr++ == MESSAGE_LENGTH-9) writeItr = 0;
        strncpy(display, DEMO_MESSAGE + writeItr, 9);
        strncpy(disdot,  DEMO_DOTFLAG + writeItr, 9);
        
        setMsgWithDot(display, disdot);
        
    }
}



enum clock_task_status_t {
    REQUEST_DATA,
    WAITING_DATA
} clock_task_status;

void clock_task() {
    if (!DIP_SHOW_TIME) return;
    
    switch (clock_task_status) {
        case REQUEST_DATA:
            if (!I2C_isAvailable()) return;
            I2C_RequestRead(0xD0, 0x00, 7);
            clock_task_status = WAITING_DATA;
            break;
        case WAITING_DATA:
            if (!I2C_isReadable()) return;
            uint8_t rtcdata[7];
            memcpy(rtcdata, I2C_ReadBuff(), 7);
            uint8_t buffer[10];
            sprintf(buffer, "%02d%1d%02d%02d%02d", bcd2dec(rtcdata[4]), bcd2dec(rtcdata[3]), bcd2dec(rtcdata[2] & 0x3F), bcd2dec(rtcdata[1]), bcd2dec(rtcdata[0] & 0x7F));
            setMsg(buffer);
            clock_task_status = REQUEST_DATA;
            break;
    }
}

enum rtc_setting_status_t {
    SETTING_YEAR,
    SETTING_MON,
    SETTING_DATE,
    SETTING_DAY,
    SETTING_HOUR,
    SETTING_MIN,
    SETTING_SEC,
    REQUESTING_WRITE,
    RTC_WRITING,
    RTC_DONE
} rtc_setting_status;

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

uint8_t buffer[10];

void rtc_setting_task() {
    if (!DIP_SET_TIME) return;
    switch (rtc_setting_status) {
        case SETTING_YEAR:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) year = (++year)%100;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) year = (--year)%100; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_MON;
            sprintf(buffer, "YEAR-20%02d", year);
            setMsg(buffer);
            break;
            
        case SETTING_MON:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) mon = (++mon)%12;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) mon = (--mon)%12; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_DATE;
            sprintf(buffer, "MON -  %2d", mon+1);
            setMsg(buffer);
            break;
            
        case SETTING_DATE:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) date = (++date)%31;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) date = (--date)%31; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_DAY;
            sprintf(buffer, "DATE-  %2d", date+1);
            setMsg(buffer);
            break;
            
        case SETTING_DAY:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) day = (++day)%7;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) day = (--day)%7; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_HOUR;
            sprintf(buffer, "DAY -  %2d", day+1);
            setMsg(buffer);
            break;
            
        case SETTING_HOUR:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) hour = (++hour)%24;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) hour = (--hour)%24; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_MIN;
            sprintf(buffer, "HOUR-  %2d", hour);
            setMsg(buffer);
            break;
            
        case SETTING_MIN:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) min = (++min)%60;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) min = (--min)%60; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) rtc_setting_status = SETTING_SEC;
            sprintf(buffer, "MIN -  %2d", min);
            setMsg(buffer);
            break;
            
        case SETTING_SEC:
            if ((BUTTON_UP != up_before) && !BUTTON_UP) sec = (++sec)%60;
            if ((BUTTON_DOWN != down_before) && !BUTTON_DOWN) sec = (--sec)%60; 
            if ((BUTTON_SEL != sel_before) && !BUTTON_SEL) {
                setMsg("WAITING..");
                rtc_setting_status = REQUESTING_WRITE;
            }
            sprintf(buffer, "SEC -  %2d", sec);
            setMsg(buffer);
            break;
            
        case REQUESTING_WRITE:
            if (!I2C_isAvailable()) break;
            uint8_t write[7];
            write[0] = dec2bcd(sec);
            write[1] = dec2bcd(min);
            write[2] = dec2bcd(hour);
            write[3] = dec2bcd(day+1);
            write[4] = dec2bcd(date+1);
            write[5] = dec2bcd(mon+1);
            write[6] = dec2bcd(year);

            I2C_WriteBuff(0xD0, 0x00, write, 7);
            rtc_setting_status = RTC_WRITING;
            setMsg("WRITING  ");
            break;
            
        case RTC_WRITING:
            if (!I2C_isAvailable()) break;
            setMsg("DONE     ");
            rtc_setting_status = RTC_DONE;
            break;
            
        case RTC_DONE:
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


//次の桁を表示する
void interrupt isr(void) {
    
    // UARTの処理
    if (PIR1bits.RCIF) {
        PIR1bits.RCIF = 0;
        
        uint8_t RxData = RCREG;
        if (uartBuffIsWriteAble()) {
												writeUartBuff(RxData);
        } else {
												//   ¯＼_(ツ)_／¯
        }
    }
    
    
    // タイマー0の処理
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
}
