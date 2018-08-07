/*******************************************************************************
  LED demo board abstraction layer for PICDEM FS USB demo board.

  Company:
    Microchip Technology Inc.

  File Name:
    leds.c

  Summary:
    LED demo board abstraction layer for PICDEM FS USB demo board.

  Description:
    LED demo board abstraction layer for PICDEM FS USB demo board.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>

#include "leds.h"
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Constants
// *****************************************************************************
// *****************************************************************************
#define LED_D1_LAT LATDbits.LATD0
#define LED_D2_LAT LATDbits.LATD1
#define LED_D3_LAT LATDbits.LATD2
#define LED_D4_LAT LATDbits.LATD3
#define LED_E2_LAT LATEbits.LATE2

#define LED_D1_TRIS TRISDbits.TRISD0
#define LED_D2_TRIS TRISDbits.TRISD1
#define LED_D3_TRIS TRISDbits.TRISD2
#define LED_D4_TRIS TRISDbits.TRISD3
#define LED_E2_TRIS TRISEbits.TRISE2

#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0


// *****************************************************************************
// *****************************************************************************
// Section: Macros or Functions
// *****************************************************************************
// *****************************************************************************

/*********************************************************************
* Function: void LED_On(LED led);
*
* Overview: Turns requested LED on
*
* PreCondition: LED configured via LED_Configure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_On(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_On(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT = LED_ON;
            break;

        case LED_D2:
            LED_D2_LAT = LED_ON;
            break;
			
        case LED_D3:
            LED_D3_LAT = LED_ON;
            break;
			
        case LED_D4:
            LED_D4_LAT = LED_ON;
            break;
            
        case LED_E2:
            LED_E2_LAT = LED_ON;
            break;

        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: void LED_Off(LED led);
*
* Overview: Turns requested LED off
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Off(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Off(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT = LED_OFF;
            break;

        case LED_D2:
            LED_D2_LAT = LED_OFF;
            break;
			
        case LED_D3:
            LED_D3_LAT = LED_OFF;
            break;
			
        case LED_D4:
            LED_D4_LAT = LED_OFF;
            break;
            
        case LED_E2:
            LED_E2_LAT = LED_OFF;
            break;

        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: void LED_Toggle(LED led);
*
* Overview: Toggles the state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Toggle(LED_CONNECTION_DETECTED);
*
* Output: none
*
********************************************************************/
void LED_Toggle(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_LAT ^= 1;
            break;

        case LED_D2:
            LED_D2_LAT ^= 1;
            break;
			
        case LED_D3:
            LED_D3_LAT ^= 1;
            break;
			
        case LED_D4:
            LED_D4_LAT ^= 1;
            break;
            
        case LED_E2:
            LED_E2_LAT ^= 1;
            break;

        case LED_NONE:
            break;
    }
}

/*********************************************************************
* Function: bool LED_Get(LED led);
*
* Overview: Returns the current state of the requested LED
*
* PreCondition: LED configured via LEDConfigure()
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*         i.e. - LED_Get(LED_CONNECTION_DETECTED);
*
* Output: true if on, false if off
*
********************************************************************/
bool LED_Get(LED led)
{
    switch(led)
    {
        case LED_D1:
            return ( (LED_D1_LAT == LED_ON) ? true : false );

        case LED_D2:
            return ( (LED_D2_LAT == LED_ON) ? true : false );
			
        case LED_D3:
            return ( (LED_D3_LAT == LED_ON) ? true : false );
			
        case LED_D4:
            return ( (LED_D4_LAT == LED_ON) ? true : false );
            
        case LED_E2:
            return ( (LED_E2_LAT == LED_ON) ? true : false );

        case LED_NONE:
            return false;
    }
    
    return false;
}

/*********************************************************************
* Function: void LED_Enable(LED led);
*
* Overview: Configures the LED for use by the other LED API
*
* PreCondition: none
*
* Input: LED led - enumeration of the LEDs available in this
*        demo.  They should be meaningful names and not the names of
*        the LEDs on the silkscreen on the board (as the demo code may
*        be ported to other boards).
*
* Output: none
*
********************************************************************/
void LED_Enable(LED led)
{
    switch(led)
    {
        case LED_D1:
            LED_D1_TRIS = OUTPUT;
            break;

        case LED_D2:
            LED_D2_TRIS = OUTPUT;
            break;
			
        case LED_D3:
            LED_D3_TRIS = OUTPUT;
            break;
			
        case LED_D4:
            LED_D4_TRIS = OUTPUT;
            break;
            
        case LED_E2:
            LED_E2_TRIS = OUTPUT;
            break;

        case LED_NONE:
            break;
    }
}


/*******************************************************************************
 End of File
*/
