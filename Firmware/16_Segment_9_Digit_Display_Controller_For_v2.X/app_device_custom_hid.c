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

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_hid.h"

#include <string.h>
#include <stdint.h>

#include "system.h"


/** VARIABLES ******************************************************/
/* Some processors have a limited range of RAM addresses where the USB module
 * is able to access.  The following section is for those devices.  This section
 * assigns the buffers that need to be used by the USB module into those
 * specific areas.
 */
#if defined(FIXED_ADDRESS_MEMORY)
    #if defined(COMPILER_MPLAB_C18)
        #pragma udata HID_CUSTOM_OUT_DATA_BUFFER = HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS
        unsigned char ReceivedDataBuffer[64];
        #pragma udata HID_CUSTOM_IN_DATA_BUFFER = HID_CUSTOM_IN_DATA_BUFFER_ADDRESS
        unsigned char ToSendDataBuffer[64];
        #pragma udata

    #else defined(__XC8)
        unsigned char ReceivedDataBuffer[64] @ HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS;
        unsigned char ToSendDataBuffer[64] @ HID_CUSTOM_IN_DATA_BUFFER_ADDRESS;
    #endif
#else
    unsigned char ReceivedDataBuffer[64];
    unsigned char ToSendDataBuffer[64];
#endif

volatile USB_HANDLE USBOutHandle;    
volatile USB_HANDLE USBInHandle;

/** DEFINITIONS ****************************************************/
typedef enum {
    COMMAND_WRITE_SEGMENT = 0x10,
    COMMAND_WRITE_SEGMENT_ALL = 0x1F,
    COMMAND_WRITE_CUSTOM = 0x20,
    COMMAND_WRITE_CUSTOM_ALL = 0x2F,
    COMMAND_READ_VERSION = 0xFF         
} CUSTOM_HID_DEMO_COMMANDS;

/** FUNCTIONS ******************************************************/

/*********************************************************************
* Function: void APP_DeviceCustomHIDInitialize(void);
*
* Overview: Initializes the Custom HID demo code
*
* PreCondition: None
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCustomHIDInitialize()
{
    //initialize the variable holding the handle for the last
    // transmission
    USBInHandle = 0;

    //enable the HID endpoint
    USBEnableEndpoint(CUSTOM_DEVICE_HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = (volatile USB_HANDLE)HIDRxPacket(CUSTOM_DEVICE_HID_EP,(uint8_t*)&ReceivedDataBuffer[0],64);
}

/*********************************************************************
* Function: void APP_DeviceCustomHIDTasks(void);
*
* Overview: Keeps the Custom HID demo running.
*
* PreCondition: The demo should have been initialized and started via
*   the APP_DeviceCustomHIDInitialize() and APP_DeviceCustomHIDStart() demos
*   respectively.
*
* Input: None
*
* Output: None
*
********************************************************************/
void APP_DeviceCustomHIDTasks() {
    /* If the USB device isn't configured yet, we can't really do anything
     * else since we don't have a host to talk to.  So jump back to the
     * top of the while loop. */
    if( USBGetDeviceState() < CONFIGURED_STATE ) {
        return;
    }

    /* If we are currently suspended, then we need to see if we need to
     * issue a remote wakeup.  In either case, we shouldn't process any
     * keyboard commands since we aren't currently communicating to the host
     * thus just continue back to the start of the while loop. */
    if( USBIsDeviceSuspended()== true ) {
        return;
    }
    
    //Check if we have received an OUT data packet from the host
    if(HIDRxHandleBusy(USBOutHandle) == false) {   
        //We just received a packet of data from the USB host.
        //Check the first uint8_t of the packet to see what command the host
        //application software wants us to fulfill.
        switch(ReceivedDataBuffer[0]) { //Look at the data the host sent, to see what kind of application specific command it sent.
            
            case COMMAND_WRITE_SEGMENT: {   // [ COMMAND | DIGIT | FontID | DOT(.......0) ]
                uint8_t DIGIT  = ReceivedDataBuffer[1];
                uint8_t FontID = ReceivedDataBuffer[2];
                uint8_t DOT    = ReceivedDataBuffer[3];
                segMap[DIGIT] = ~(fontList[FontID] | ((uint32_t)DOT << 16));
                break;
            }
            case COMMAND_WRITE_SEGMENT_ALL: {// [ COMMAND | FontID(0) | FontID(1) | FontID(2) | FontID(3) | FontID(4) | FontID(5) | FontID(6) | FontID(7) | FontID(8) | DOT(7-0) | DOT(.......8)
                uint8_t Dot7_0 = ReceivedDataBuffer[10];
                uint8_t Dot_8  = ReceivedDataBuffer[11];
                for (int i = 0; i < 8; i++) {
                    segMap[i] = ~(fontList[ReceivedDataBuffer[i+1]] | (((uint32_t)Dot7_0&(0b1<<i)) << 16-i));
                }
                segMap[8] = ~(fontList[ReceivedDataBuffer[9]] | (((uint32_t)Dot_8&0b00000001) << 16));
                break;
            }
            case COMMAND_WRITE_CUSTOM: {    // [ COMMAND | DIGIT | CUSTOM(MSB) | CUSTOM(LSB) | DOT(.......0)]
                uint8_t DIGIT   = ReceivedDataBuffer[1];
                uint8_t CustomH = ReceivedDataBuffer[2];
                uint8_t CustomL = ReceivedDataBuffer[3];
                uint8_t DOT     = ReceivedDataBuffer[4];
                segMap[DIGIT] = ~((CustomH << 8) | CustomL | ((uint32_t)DOT << 16));
                break;
            }
            case COMMAND_WRITE_CUSTOM_ALL: {// [ COMMAND | CUSTOM(0)H,L | CUSTOM(1)H,L | CUSTOM(2)H,L | CUSTOM(3)H,L | CUSTOM(4)H,L | CUSTOM(5)H,L | CUSTOM(6)H,L | CUSTOM(7)H,L | CUSTOM(8)H,L | Dot(7-0) | DOT(.......8)]
                uint8_t Dot7_0 = ReceivedDataBuffer[19];
                uint8_t Dot_8  = ReceivedDataBuffer[20];
                for (int i = 0; i < 8; i++) {
                    segMap[i] = ~((ReceivedDataBuffer[((i+1)*2)-1] << 8) | ReceivedDataBuffer[(i+1)*2] | ((uint32_t)ReceivedDataBuffer[19] << 16-i));
                }
                segMap[8] = ~((ReceivedDataBuffer[17] << 8) | ReceivedDataBuffer[18] | ((uint32_t)ReceivedDataBuffer[20] << 16));
                break;
            }
            case COMMAND_READ_VERSION: {
                ToSendDataBuffer[0] = 0xFF;
                ToSendDataBuffer[1] = 0x02;
                //Prepare the USB module to send the data packet to the host
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                break;
            }
        }
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet 
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ReceivedDataBuffer[0], 64);
    }
}