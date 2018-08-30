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

#ifndef SYSTEM_H
#define SYSTEM_H

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "i2c.h"
#include "usb.h"
#include "fixed_address_memory.h"
#include "utilities.h"

#define _XTAL_FREQ 20000000

#define MAIN_RETURN void

/*** System States **************************************************/
typedef enum {
    SYSTEM_STATE_USB_START,
    SYSTEM_STATE_USB_SUSPEND,
    SYSTEM_STATE_USB_RESUME
} SYSTEM_STATE;

const uint8_t  DEMO_MESSAGE[] =  "MAKER FAIRE TOKYO 2018 HTLABNET BOOTH! THIS IS 16 SEGMENT 9 DIGIT DISPLAY        ";
const uint8_t  DEMO_DOTFLAG[] =  "                           .                                                     ";
const uint16_t MESSAGE_LENGTH = (int)(sizeof(DEMO_MESSAGE)/sizeof(char));

#define BUTTON_UP   PORTEbits.RE0
#define BUTTON_DOWN PORTEbits.RE1
#define BUTTON_SEL  PORTEbits.RE2

extern uint8_t digitPtr; // 現在表示している桁数
extern bool    showDemoMessage;

extern uint8_t led_stat;     // コントローラについてる4つのbarLEDの点灯状態

//初期値"*********"
extern uint32_t segMap[9];

void handleMessage();
void refreshShiftRegister(int ptr);
void clock_task();

#endif //SYSTEM_H
