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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include "system.h"
#include "usb_device.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

void APP_LEDUpdateUSBStatus(void) {
    static uint16_t ledCount = 0;

    if(USBIsDeviceSuspended() == true) {
        led_stat = led_stat | 0b00000001;
        return;
    }

    switch(USBGetDeviceState())
    {         
        case CONFIGURED_STATE:
            /* We are configured.  Blink fast.
             * On for 75ms, off for 75ms, then reset/repeat. */
            if(ledCount == 1) {
                led_stat = led_stat & 0b11111110;
            }
            else if(ledCount == 75) {
                led_stat = led_stat | 0b00000001;
            }
            else if(ledCount > 150) {
                ledCount = 0;
            }
            /* We are configured */
            break;

        default:
            /* We aren't configured yet, but we aren't suspended so let's blink with
             * a slow pulse. On for 50ms, then off for 950ms, then reset/repeat. */
            if(ledCount == 1) {
                led_stat = led_stat & 0b11111110;
            }
            else if(ledCount == 50) {
                led_stat = led_stat | 0b00000001;
            }
            else if(ledCount > 950) {
                ledCount = 0;
            }
            /* We aren't configured yet */
            break;
    }

    /* Increment the millisecond counter. */
    ledCount++;
}

/*******************************************************************************
 End of File
*/
