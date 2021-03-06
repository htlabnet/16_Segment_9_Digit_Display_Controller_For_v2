//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//	main.c
//	16 segment 9 digit display Controller 実体プログラム
//
//	Created by thotgamma. (https://gammalab.net)
//
//	16Segment9DigitDisplay公式:
//		https://htlab.net/products/electronics/16-segment-9-digit-display-board-1/
//		https://htlab.net/products/electronics/16-segment-9-digit-display-controller-1/
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=


#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "system.h"
#include "usb_device_hid.h"
#include "app_device_custom_hid.h"
#include "app_led_usb_status.h"


uint16_t led_target_voltage = 8000;     //mV
uint16_t led_voltage_threshold = 1000;  //mV

void pwmInit() {
    T2CON = 0;
    T2CONbits.T2CKPS = 0b01;    // Prescaler is 4
    PR2 = 64;   // 19.2kHz
    CCPR1L = 1;
    // No Load Voltage (VCC:5.0V @ T2CKPS = 0b01 & PR2 = 64)
    // CCPR1L = 1 :  5.81V
    // CCPR1L = 2 :  7.70V
    // CCPR1L = 3 :  9.76V
    // CCPR1L = 4 : 11.85V
    // CCPR1L = 5 : 13.92V
    // CCPR1L = 6 : 15.98V
    CCP1CON = 0b00001100;
    TMR2 = 0;
    TMR2ON = 1;
}
 
void pwmSet(uint8_t ontime) {
    if (ontime < PR2 - 40) {    // (max duty : 64(PR2) - 40 = 24)
        CCPR1L = ontime;
    }
}

void pwmSetInc() {
    if (CCPR1L < PR2 - 40) {    // (max duty : 64(PR2) - 40 = 24)
        CCPR1L = CCPR1L + 1;
    }
}

void pwmSetDec() {
    if (CCPR1L > 1) {
        CCPR1L = CCPR1L - 1;
    }
}

void adcInit() {
    ADCON1 = 0b00001110;
    ADCON2 = 0b10010010;
    ADRESH = 0;
    ADRESL = 0;   
}

uint16_t adcRead(uint8_t channel) {
    ADCON0 = (ADCON0 & 0b11000011) | ((channel<<2) & 0b00111100);
    ADCON0 |= ((1<<ADON) | (1<<GO));
    while(ADCON0bits.GO_nDONE==1);
    return ((ADRESH*256) | (ADRESL));
}

uint16_t adcReadVcc() {
    // Ref 2.495V
    // ref_adc = adcRead(3); // 0-4095 (VCC 5V at about 2043)
    // 2043 * 5000(mV) = 10215000
    return (10215000 / adcRead(3));     // return vcc millivolts
}

uint16_t adcReadLed() {
    // LED 10:1 (12V -> 1.09V)
    // led_adc = adcRead(1); // 0-4095 (LED 12V at about 900)
    uint32_t led;
    led = (uint32_t)adcReadVcc() * (uint32_t)adcRead(1) * 11; // 11:divide
    led /= 4096;
    return (uint16_t)led;        // return led millivolts
}

void main(void) {

    ADCON1 = 0b00001111; //All Digital
    CMCON  = 0b00000111; //No Comparator
    TRISA  = 0b00001111;
    TRISB  = 0b00000011;
    TRISC  = 0b10000000; //Rxのみ入力
    TRISD  = 0b11111111;
    TRISE  = 0b00000111;
    
    PORTEbits.RDPU = 1;

    LATCbits.LATC0 = 0; // OUTPUT ENABLE
    LATCbits.LATC1 = 1; // CLEAR

    LATCbits.LATC2 = 0; // PWM

    // 12VDCジャックアリのときはどっちも1
    LATAbits.LATA4 = 0; // USB_VCC -> VCC
    LATAbits.LATA5 = 0; // VCC -> LED
    
    // LED制御のタイマー設定
    T0CON = 0b11000101; //Timer0 ON, 8Bit Mode, InternalClock, 1/64 Prescale
    TMR0 = 0xFF - 125+1;
    
    I2C_Init();

    //各種割り込み許可
    INTCONbits.TMR0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    // UART設定
    TXSTA   = 0b00000000;
    RCSTA   = 0b10010000;
    BAUDCON = 0b00000000;
    SPBRG   = 77;
    PIE1bits.RCIE = 1;
    
    if (DIP_SHOW_ALL) {
        for(int i = 0; i < 9; i++) {
            segMap[i] = 0b100000000000000000;
        }
    }
    showDemoMessage = DIP_SHOW_DEMO;
    
    // Enable ADC
    adcInit();
    
    // Enable DC-DC Converter
    pwmInit();
    
    // if (adcRead(0) > 700) {pwmDisable} //12V DC入力時に900程度？
    
    USBDeviceInit();
    USBDeviceAttach();
    
    while (1){
        
        // DC-DC Converter
        if (adcReadLed() < led_target_voltage - led_voltage_threshold) {
            pwmSetInc();
        } else if (adcReadLed() > led_target_voltage + led_voltage_threshold) {
            pwmSetDec();
        }
        
        APP_DeviceCustomHIDTasks();
        uart_task();
        I2C_Task();
        clock_task();
        rtc_setting_task();
    }
}
