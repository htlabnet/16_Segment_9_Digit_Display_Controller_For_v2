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

/** I N C L U D E S **********************************************************/
#include "usb.h"
#include "BootPIC18NonJ.h"



/** C O N S T A N T S **********************************************************/
//The bootloader version, which the bootloader PC application can do extended query to get.
//Value provided is expected to be in the format of BOOTLOADER_VERSION_MAJOR.BOOTLOADER_VERSION_MINOR
//Ex: 1.01 would be BOOTLOADER_VERSION_MAJOR == 1, and BOOTLOADER_VERSION_MINOR == 1
#define BOOTLOADER_VERSION_MAJOR         1 //Legal value 0-255
#define BOOTLOADER_VERSION_MINOR         2 //Legal value 0-99.  (1 = X.01)


//Section defining the address range to erase for the erase device command, along with the valid programming range to be reported by the QUERY_DEVICE command.
#define PROGRAM_MEM_START_ADDRESS       REMAPPED_APPLICATION_RESET_VECTOR //Beginning of application program memory (not occupied by bootloader).  **THIS VALUE MUST BE ALIGNED WITH 64 BYTE BLOCK BOUNDRY** Also, in order to work correctly, make sure the StartPageToErase is set to erase this section.

#if defined(__18F4550)||defined(__18F2550)||defined(__18F45K50)||defined(__18LF45K50)||defined(__18F25K50)||defined(__18LF25K50)
    #define MAX_PAGE_TO_ERASE           511      //Last 64 byte page of flash on the PIC18F4550
    #define PROGRAM_MEM_STOP_ADDRESS    0x008000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 //0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x20     //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
    #define ERASE_PAGE_SIZE             64
    
#elif defined(__18F4553)||defined(__18F2553)
    #define MAX_PAGE_TO_ERASE           511      //Last 64 byte page of flash on the PIC18F4550
    #define PROGRAM_MEM_STOP_ADDRESS    0x008000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 //0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x20     //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
    #define ERASE_PAGE_SIZE             64
    
#elif defined(__18F4455)||defined(__18F2455)
    #define MAX_PAGE_TO_ERASE           383      //Last 64 byte page of flash on the PIC18F4455
    #define PROGRAM_MEM_STOP_ADDRESS    0x006000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 /0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x20     //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
    #define ERASE_PAGE_SIZE             64

#elif defined(__18F4458)||defined(__18F2458)
    #define MAX_PAGE_TO_ERASE           383      //Last 64 byte page of flash on the PIC18F4455
    #define PROGRAM_MEM_STOP_ADDRESS    0x006000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 /0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define UserIDSize                  8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x20     //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
    #define ERASE_PAGE_SIZE             64

#elif defined(__18F4450)||defined(__18F2450)
    #define MAX_PAGE_TO_ERASE           255      //Last 64 byte page of flash on the PIC18F4450
    #define PROGRAM_MEM_STOP_ADDRESS    0x004000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 /0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define WRITE_BLOCK_SIZE            0x10     //16 byte programming block size on the PIC18F4450/2450 family devices
    #define ERASE_PAGE_SIZE             64

#elif defined(__18F24K50)||defined(__18LF24K50) 
    #define MAX_PAGE_TO_ERASE           255      //Last 64 byte page of flash on the PIC18F4450
    #define PROGRAM_MEM_STOP_ADDRESS    0x004000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 /0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x10     //16 byte programming block size on the PIC18F4450/2450 family devices
    #define ERASE_PAGE_SIZE             64

#elif defined(__18F14K50) || defined(__18LF14K50)
    #define MAX_PAGE_TO_ERASE           255      //Last 64 byte page of flash on the PIC18F4455
    #define PROGRAM_MEM_STOP_ADDRESS    0x004000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 //0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x10     //16 byte programming block size on the PIC18F14K50 family devices
    #define ERASE_PAGE_SIZE             64

#elif defined(__18F13K50) || defined(__18LF13K50)
    #define MAX_PAGE_TO_ERASE           127      //Last 64 byte page of flash on the PIC18F4455
    #define PROGRAM_MEM_STOP_ADDRESS    0x002000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If PROGRAM_MEM_STOP_ADDRESS = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**
    #define CONFIG_WORDS_START_ADDRESS  0x300000 //0x300000 is start of CONFIG space for these devices
    #define CONFIG_WORDS_SECTION_LENGTH 14       //14 bytes worth of Configuration words on these devices
    #define USER_ID_ADDRESS             0x200000 //User ID is 8 bytes starting at 0x200000
    #define USER_ID_SIZE                8
    #define DEVICE_WITH_EEPROM                   //Comment this out, if you never want the bootloader to reprogram the EEPROM space
    #define EEPROM_SIZE                 0x100    //256 bytes of EEPROM on this device
    #define EEPROM_EFFECTIVE_ADDRESS    0xF00000 //Location in the .hex file where the EEPROM contents are stored
    #define WRITE_BLOCK_SIZE            0x08     //8 byte programming block size on the PIC18F14K50 family devices
    #define ERASE_PAGE_SIZE             64

#endif


//Derived microcontroller address/page constants
#define START_PAGE_TO_ERASE             (PROGRAM_MEM_START_ADDRESS/ERASE_PAGE_SIZE)             //The first flash page number to erase, which is the start of the application program space
#define ERASE_PAGE_ADDRESS_MASK         (uint24_t)(0x1000000 - ERASE_PAGE_SIZE)                 //AND mask to move an address pointer to the start of an erase page



//Bootloader Command From Host - Switch() State Variable Choices
#define QUERY_DEVICE                0x02    //Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define UNLOCK_CONFIG               0x03    //Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE                0x04    //Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define PROGRAM_DEVICE              0x05    //If host is going to send a full RequestDataBlockSize to be programmed, it uses this command.
#define PROGRAM_COMPLETE            0x06    //If host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA                    0x07    //The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define RESET_DEVICE                0x08    //Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)
#define SIGN_FLASH                  0x09    //The host PC application should send this command after the verify operation has completed successfully.  If checksums are used instead of a true verify (due to ALLOW_GET_DATA_COMMAND being commented), then the host PC application should send SIGN_FLASH command after is has verified the checksums are as exected. The firmware will then program the SIGNATURE_WORD into flash at the SIGNATURE_ADDRESS.
#define QUERY_EXTENDED_INFO         0x0C    //Used by host PC app to get additional info about the device, beyond the basic NVM layout provided by the query device command

//Unlock Configs Command Definitions
#define UNLOCKCONFIG                0x00    //Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG                  0x01    //Sub-command for the ERASE_DEVICE command

//Query Device Response "Types" 
#define MEMORY_REGION_PROGRAM_MEM   0x01    //When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define MEMORY_REGION_EEDATA        0x02
#define MEMORY_REGION_CONFIG        0x03
#define MEMORY_REGION_USERID        0x04
#define MEMORY_REGION_END           0xFF    //Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.
#define BOOTLOADER_V1_01_OR_NEWER_FLAG   0xA5   //Tacked on in the VersionFlag byte, to indicate when using newer version of bootloader with extended query info available


//BootState Variable States
#define IDLE                        0x00
#define NOT_IDLE                    0x01

//OtherConstants
#define INVALID_ADDRESS             0xFFFFFFFF
#define CORRECT_UNLOCK_KEY          0xB5

//Application and Microcontroller constants
#define BYTES_PER_ADDRESS_PIC18     0x01        //One byte per address.  PIC24 uses 2 bytes for each address in the hex file.
#define USB_PACKET_SIZE             0x40
#define WORDSIZE                    0x02    //PIC18 uses 2 byte words, PIC24 uses 3 byte words.
#define REQUEST_DATA_BLOCK_SIZE     0x3A    //Number of data bytes in a standard request to the PC.  Must be an even number from 2-58 (0x02-0x3A).  Larger numbers make better use of USB bandwidth and 
                                            //yeild shorter program/verify times, but require more micrcontroller RAM for buffer space.



/** USB Packet Request/Response Formatting Structure **********************************************************/
typedef union 
{
        unsigned char Contents[USB_PACKET_SIZE];

        //General command (with data in it) packet structure used by PROGRAM_DEVICE and GET_DATA commands       
        struct{
            unsigned char Command;
            unsigned long Address;
            unsigned char Size;
            //unsigned char PadBytes[58-REQUEST_DATA_BLOCK_SIZE]; //Uncomment this if using a smaller than 0x3A RequestDataBlockSize.  Compiler doesn't like 0 byte array when using 58 byte data block size.
            unsigned char Data[REQUEST_DATA_BLOCK_SIZE];
        };
        
        //This struct used for responding to QUERY_DEVICE command (on a device with four programmable sections)
        struct{
            unsigned char Command;
            unsigned char PacketDataFieldSize;
            unsigned char BytesPerAddress;
            unsigned char Type1;
            unsigned long Address1;
            unsigned long Length1;
            unsigned char Type2;
            unsigned long Address2;
            unsigned long Length2;
            unsigned char Type3;
            unsigned long Address3;
            unsigned long Length3;
            unsigned char Type4;
            unsigned long Address4;
            unsigned long Length4;                      
            unsigned char Type5;
            unsigned long Address5;
            unsigned long Length5;
            unsigned char Type6;
            unsigned long Address6;
            unsigned long Length6;  
            unsigned char VersionFlag;      //Used by host software to identify if device is new enough to support QUERY_EXTENDED_INFO command  
            unsigned char ExtraPadBytes[7];
        };  
        
        struct{                     //For UNLOCK_CONFIG command
            unsigned char Command;
            unsigned char LockValue;
        };
        
        //Structure for the QUERY_EXTENDED_INFO command (and response)
        struct{
            unsigned char Command;
            unsigned int BootloaderVersion;
            unsigned int ApplicationVersion;
            unsigned long SignatureAddress;
            unsigned int SignatureValue;
            unsigned long ErasePageSize;
            unsigned char Config1LMask;
            unsigned char Config1HMask;
            unsigned char Config2LMask;
            unsigned char Config2HMask;
            unsigned char Config3LMask;
            unsigned char Config3HMask;
            unsigned char Config4LMask;
            unsigned char Config4HMask;
            unsigned char Config5LMask;
            unsigned char Config5HMask;
            unsigned char Config6LMask;
            unsigned char Config6HMask;
            unsigned char Config7LMask;
            unsigned char Config7HMask;
        };          
} PacketToFromPC;       
    

/** V A R I A B L E S ********************************************************/
#ifndef __XC8__
#pragma udata
#endif
PacketToFromPC PacketFromPC;
#ifndef __XC8__
#pragma udata SomeSectionName3
#endif
PacketToFromPC PacketToPC;
#ifndef __XC8__
#pragma udata WriteBufferSection
#endif
unsigned char ProgrammingBuffer[ERASE_PAGE_SIZE];
#ifndef __XC8__
#pragma udata OtherVariablesSection
#endif
unsigned char BootState;
unsigned int ErasePageTracker;
unsigned char BufferedDataIndex;
uint24_t ProgrammedPointer;
unsigned char ConfigsLockValue;


/** P R I V A T E  P R O T O T Y P E S ***************************************/
void WriteFlashBlock(void);
void WriteConfigBits(void);
void WriteEEPROM(void);
void UnlockAndActivate(unsigned char UnlockKey);
void ResetDeviceCleanly(void);
void TableReadPostIncrement(void);
void SignFlash(void);
void LowVoltageCheck(void);


/** D E C L A R A T I O N S **************************************************/
#ifndef __XC8__
#pragma code
#endif
void UserInit(void)
{
    //Initialize bootloader state variables
    BootState = IDLE;
    ProgrammedPointer = INVALID_ADDRESS;    
    BufferedDataIndex = 0;
    ConfigsLockValue = TRUE;
}//end UserInit

/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function receives/sends USB packets to/from the USB 
 *                  host.  It also processes any received OUT packets and
 *                  is reponsible for generating USB IN packet data.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{
    static unsigned char i;
    static ROM uint8_t* pROM;

    //Checks for and processes application related USB packets (assuming the
    //USB bus is in the CONFIGURED_STATE, which is the only state where
    //the host is allowed to send application related USB packets to the device.
    if((USBGetDeviceState() != CONFIGURED_STATE) || (USBIsDeviceSuspended() == 1))
    {
        //No point to trying to run the application code until the device has
        //been configured (finished with enumeration) and is not currently suspended.
        return;
    }

    //Check the current bootloader state (if we are currently waiting from a new
    //command to process from the host, or if we are still processing a previous
    //command.
    if(BootState == IDLE)
    {
        //We are currently in the IDLE state waiting for a command from the
        //PC software on the USB host.
        if(!mHIDRxIsBusy()) //Did we receive a command?
        {
            //We received a new command from the host.  Copy the OUT packet from 
            //the host into a local buffer for processing.
            HIDRxReport((char *)&PacketFromPC, USB_PACKET_SIZE);     //Also re-arms the OUT endpoint to be able to receive the next packet
            BootState = NOT_IDLE;   //Set flag letting state machine know it has a command that needs processing.
            
            //Pre-initialize a response packet buffer (only used for some commands)
            for(i = 0; i < USB_PACKET_SIZE; i++)        //Prepare the next packet we will send to the host, by initializing the entire packet to 0x00.
                PacketToPC.Contents[i] = 0;             //This saves code space, since we don't have to do it independently in the QUERY_DEVICE and GET_DATA cases.
        }
    }//if(BootState == IDLE)
    else //(BootState must be NOT_IDLE)
    {   
        //Check the latest command we received from the PC app, to determine what
        //we should be doing.
        switch(PacketFromPC.Command)
        {
            case QUERY_DEVICE:
                //Make sure the USB IN endpoint buffer is available, then load
                //up a query response packet to send to the host.
                if(!mHIDTxIsBusy())
                {
                    //Prepare a response packet, which lets the PC software know about the memory ranges of this device.
                    PacketToPC.Command = QUERY_DEVICE;
                    PacketToPC.PacketDataFieldSize = REQUEST_DATA_BLOCK_SIZE;
                    PacketToPC.BytesPerAddress = BYTES_PER_ADDRESS_PIC18;
                    PacketToPC.Type1 = MEMORY_REGION_PROGRAM_MEM;
                    PacketToPC.Address1 = (unsigned long)PROGRAM_MEM_START_ADDRESS;
                    PacketToPC.Length1 = (unsigned long)(PROGRAM_MEM_STOP_ADDRESS - PROGRAM_MEM_START_ADDRESS); //Size of program memory area
                    PacketToPC.Type2 = MEMORY_REGION_CONFIG;
                    PacketToPC.Address2 = (unsigned long)CONFIG_WORDS_START_ADDRESS;
                    PacketToPC.Length2 = (unsigned long)CONFIG_WORDS_SECTION_LENGTH;
                    PacketToPC.Type3 = MEMORY_REGION_USERID;        //Not really program memory (User ID), but may be treated as it it was as far as the host is concerned
                    PacketToPC.Address3 = (unsigned long)USER_ID_ADDRESS;
                    PacketToPC.Length3 = (unsigned long)(USER_ID_SIZE);
                    PacketToPC.Type4 = MEMORY_REGION_END;
                    #if defined(DEVICE_WITH_EEPROM)
                        PacketToPC.Type4 = MEMORY_REGION_EEDATA;
                        PacketToPC.Address4 = (unsigned long)EEPROM_EFFECTIVE_ADDRESS;
                        PacketToPC.Length4 = (unsigned long)EEPROM_SIZE;
                        PacketToPC.Type5 = MEMORY_REGION_END;
                    #endif
                    PacketToPC.VersionFlag = BOOTLOADER_V1_01_OR_NEWER_FLAG;
                    //Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).

                    //Now send the packet to the USB host software, assuming the USB endpoint is available/ready to accept new data.
                    HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
                    BootState = IDLE;
                }
                break;
            case UNLOCK_CONFIG:
                ConfigsLockValue = TRUE;
                if(PacketFromPC.LockValue == UNLOCKCONFIG)
                {
                    ConfigsLockValue = FALSE;
                }
                BootState = IDLE;
                break;
            case ERASE_DEVICE:
                //First erase main program flash memory
                for(ErasePageTracker = START_PAGE_TO_ERASE; ErasePageTracker < (unsigned int)(MAX_PAGE_TO_ERASE + 1); ErasePageTracker++)
                {
                    ClearWatchdog();
                    #ifdef __XC8__
                        TBLPTRU = 0x00;
                        TBLPTRH = (uint8_t)((uint24_t)ErasePageTracker >> 2);
                        TBLPTRL = (uint8_t)((uint24_t)ErasePageTracker << 6);
                    #else
                        TBLPTR = ErasePageTracker << 6;
                    #endif
                    EECON1 = 0b10010100;    //Prepare for erasing flash memory
                    UnlockAndActivate(CORRECT_UNLOCK_KEY);
                    USBDeviceTasks();     //Call USBDeviceTasks() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
                }
                
                #if defined(DEVICE_WITH_EEPROM)
                    //Now erase EEPROM (if any is present on the device)
                    i = EEPROM_EFFECTIVE_ADDRESS & (EEPROM_SIZE-1);
                    do{
                        EEADR = i;
                        EEDATA = 0xFF;
                        EECON1 = 0b00000100;    //EEPROM Write mode
                        USBDeviceTasks();     //Call USBDeviceTasks() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
                        UnlockAndActivate(CORRECT_UNLOCK_KEY);                  
                    }while(i++<((EEPROM_SIZE-1)+(EEPROM_EFFECTIVE_ADDRESS & (EEPROM_SIZE-1))));
                #endif

                //Now erase the User ID space (0x200000 to 0x200007)
                //TBLPTR = USER_ID_ADDRESS;
                TBLPTRU = 0x20;
                TBLPTRH = 0x00;
                TBLPTRL = (uint8_t)USER_ID_ADDRESS;
                EECON1 = 0b10010100;    //Prepare for erasing flash memory
                UnlockAndActivate(CORRECT_UNLOCK_KEY);

                BootState = IDLE;               
                break;
            case PROGRAM_DEVICE:
                //Check if host is trying to program the config bits
                if(PacketFromPC.Contents[3] == 0x30) //             //PacketFromPC.Contents[3] is bits 23:16 of the address.  
                {                                                   //0x30 implies config bits
                    if(ConfigsLockValue == FALSE)
                    {
                        WriteConfigBits();      //Doesn't get reprogrammed if the UNLOCK_CONFIG (LockValue = UNLOCKCONFIG) command hasn't previously been sent
                    }
                    BootState = IDLE;
                    break;
                }

                #if defined(DEVICE_WITH_EEPROM)
                    //Check if host is trying to program the EEPROM
                    if(PacketFromPC.Contents[3] == 0xF0)    //PacketFromPC.Contents[3] is bits 23:16 of the address.  
                    {                                       //0xF0 implies EEPROM
                        WriteEEPROM();
                        BootState = IDLE;
                        break;
                    }
                #endif

                if(ProgrammedPointer == (uint24_t)INVALID_ADDRESS)
                    ProgrammedPointer = PacketFromPC.Address;
                
                if(ProgrammedPointer == (uint24_t)PacketFromPC.Address)
                {
                    for(i = 0; i < PacketFromPC.Size; i++)
                    {
                        ProgrammingBuffer[BufferedDataIndex] = PacketFromPC.Data[i+(REQUEST_DATA_BLOCK_SIZE-PacketFromPC.Size)];    //Data field is right justified.  Need to put it in the buffer left justified.
                        BufferedDataIndex++;
                        ProgrammedPointer++;
                        if(BufferedDataIndex == WRITE_BLOCK_SIZE)
                        {
                            WriteFlashBlock();
                        }
                    }
                }
                //else host sent us a non-contiguous packet address...  to make 
                //this firmware simpler, host should not do this without sending 
                //a PROGRAM_COMPLETE command in between program sections.
                BootState = IDLE;
                break;
            case PROGRAM_COMPLETE:
                WriteFlashBlock();
                ProgrammedPointer = INVALID_ADDRESS;        //Reinitialize pointer to an invalid range, so we know the next PROGRAM_DEVICE will be the start address of a contiguous section.
                BootState = IDLE;
                break;
            case GET_DATA:
                //Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
                PacketToPC.Command = GET_DATA;
                PacketToPC.Address = PacketFromPC.Address;
                PacketToPC.Size = PacketFromPC.Size;

                pROM = (ROM uint8_t*)PacketFromPC.Address;
                for(i = 0; i < PacketFromPC.Size; i++)
                {
                    if(PacketFromPC.Contents[3] == 0xF0)    //PacketFromPC.Contents[3] is bits 23:16 of the address.  
                    {                                       //0xF0 implies EEPROM, which doesn't use the table pointer to read from
                        #if defined(DEVICE_WITH_EEPROM)
                        EEADR = (((unsigned char)PacketFromPC.Address) + i);    //The bits 7:0 are 1:1 mapped to the EEPROM address space values
                        EECON1 = 0b00000000;    //EEPROM read mode
                        EECON1bits.RD = 1;
                        PacketToPC.Data[i+((USB_PACKET_SIZE - 6) - PacketFromPC.Size)] = EEDATA;                    
                        #endif
                    }
                    else    //else must have been a normal program memory region, or one that can be read from with the table pointer
                    {
                        PacketToPC.Data[i+((USB_PACKET_SIZE - 6) - PacketFromPC.Size)] = *pROM++;
                    }
                }
                
                //Assuming the USB IN (to host) buffer is available/ready, copy the
                //data over so it can get sent to the USB host software.
                if(!mHIDTxIsBusy())
                {
                    HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
                    BootState = IDLE;
                }
                break;
            case SIGN_FLASH:
                SignFlash();
                BootState = IDLE;
                break;
            case QUERY_EXTENDED_INFO:
                //Prepare a response packet with the QUERY_EXTENDED_INFO response info in it.
                //This command is only supported in bootloader firmware verison 1.01 or later.
                //Make sure the regular QUERY_DEVIER reponse packet value "PacketToPC.Type6" is = BOOTLOADER_V1_01_OR_NEWER_FLAG;
                //to let the host PC software know that the QUERY_EXTENDED_INFO command is implemented
                //in this firmware and is available for requesting by the host software.
                PacketToPC.Command = QUERY_EXTENDED_INFO;   //Echo the command byte
                PacketToPC.BootloaderVersion = ((unsigned int)BOOTLOADER_VERSION_MAJOR << 8)| BOOTLOADER_VERSION_MINOR;
                PacketToPC.ApplicationVersion = *(ROM unsigned int*)APP_VERSION_ADDRESS;
                PacketToPC.SignatureAddress = APP_SIGNATURE_ADDRESS;
                PacketToPC.SignatureValue = APP_SIGNATURE_VALUE;
                PacketToPC.ErasePageSize = ERASE_PAGE_SIZE;
                PacketToPC.Config1LMask = 0xFF;
                PacketToPC.Config1HMask = 0xFF;
                PacketToPC.Config2LMask = 0xFF;
                PacketToPC.Config2HMask = 0xFF;
                PacketToPC.Config3LMask = 0x00;
                PacketToPC.Config3HMask = 0xFF;
                PacketToPC.Config4LMask = 0xFF;
                PacketToPC.Config4HMask = 0x00;
                PacketToPC.Config5LMask = 0xFF;
                PacketToPC.Config5HMask = 0xFF;
                PacketToPC.Config6LMask = 0xFF;
                PacketToPC.Config6HMask = 0xFF;
                PacketToPC.Config7LMask = 0xFF;
                PacketToPC.Config7HMask = 0xFF;
                
                //Now actually command USB to send the packet to the host                   
                if(!mHIDTxIsBusy())
                {
                    HIDTxReport((char *)&PacketToPC, USB_PACKET_SIZE);
                    BootState = IDLE;   //Packet will be sent, go back to idle state ready for next command from host
                }       
                break;
            case RESET_DEVICE:
                ResetDeviceCleanly();
                //break;    //no need, commented to save space
            default:
                //Should never hit the default
                BootState = IDLE;
        }//End switch
    }//End of else of if(BootState == IDLE)

}//End ProcessIO()



//Should be called once, only after the regular erase/program/verify sequence 
//has completed successfully.  This function will program the magic
//APP_SIGNATURE_VALUE into the magic APP_SIGNATURE_ADDRESS in the application
//flash memory space.  This is used on the next bootup to know that the the
//flash memory image of the application is intact, and can be executed.
//This is useful for recovery purposes, in the event that an unexpected
//failure occurs during the erase/program sequence (ex: power loss or user
//unplugging the USB cable).
void SignFlash(void)
{
    static unsigned char i;
    static ROM uint8_t* pROM;
    
    //First read in the erase page contents of the page with the signature WORD
    //in it, and temporarily store it in a RAM buffer.
    pROM = (ROM uint8_t*)(APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK);
    for(i = 0; i < ERASE_PAGE_SIZE; i++)
    {
        ProgrammingBuffer[i] = *pROM++;
    }    
    
    //Now change the signature WORD value at the correct address in the RAM buffer
    ProgrammingBuffer[(APP_SIGNATURE_ADDRESS & ~ERASE_PAGE_ADDRESS_MASK)] = (unsigned char)APP_SIGNATURE_VALUE;
    ProgrammingBuffer[(APP_SIGNATURE_ADDRESS & ~ERASE_PAGE_ADDRESS_MASK) + 1] = (unsigned char)(APP_SIGNATURE_VALUE >> 8);
    
    //Now erase the flash memory block with the signature WORD in it
    //TBLPTR = APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK;
    TBLPTRU = (uint8_t)((APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK) >> 16);
    TBLPTRH = (uint8_t)((APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK) >> 8);
    TBLPTRL = (uint8_t)(APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK);

    EECON1 = 0x94;  //Prepare for flash erase operation
    UnlockAndActivate(CORRECT_UNLOCK_KEY);
    
    //Now re-program the values from the RAM buffer into the flash memory.  Use
    //reverse order, so we program the larger addresses first.  This way, the 
    //write page with the flash signature word is the last page that gets 
    //programmed (assuming the flash signature resides on the lowest address
    //write page, which is recommended, so that it becomes the first page
    //erased, and the last page programmed).
    pROM = (ROM uint8_t*)((APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK) + ERASE_PAGE_SIZE - 1);   //Point to last byte on the erase page
    //TBLPTR = (APP_SIGNATURE_ADDRESS & ERASE_PAGE_ADDRESS_MASK) + ERASE_PAGE_SIZE - 1;   //Point to last byte on the erase page
    i = ERASE_PAGE_SIZE - 1;
    while(1)
    {
        #ifdef __XC8__
            TBLPTRU = (uint32_t)pROM >> 16;
            TBLPTRH = (uint16_t)pROM >> 8;
            TBLPTRL = (uint8_t)pROM;
        #else
            TBLPTR = (uint24_t)pROM;
        #endif
        TABLAT = ProgrammingBuffer[i];

        #ifdef __XC8__
            #asm
                tblwt
            #endasm
        #else //must be C18 instead
            _asm tblwt _endasm
        #endif
                
        //Check if we are at a program write block size boundary
        if((i % WRITE_BLOCK_SIZE) == 0)
        {
            //The write latches are full, time to program the block.
            ClearWatchdog();
            EECON1 = 0xA4;  //Write to flash on next WR = 1 operation
            UnlockAndActivate(CORRECT_UNLOCK_KEY);
        }    
        //Move ROM pointer back to next location
        pROM--;
        #ifdef __XC8__
            #asm
                tblrdpostdec
            #endasm
        #else //must be C18 instead
            _asm tblrdpostdec _endasm
        #endif
        
        //Check if we are done writing all blocks
        if(i == 0)
        {
            break;  //Exit while loop
        }    
        i--;
    }    
}    


//Before resetting the microcontroller, we should shut down the USB module 
//gracefully, to make sure the host correctly recognizes that we detached
//from the bus.  Some USB hosts malfunction/fail to re-enumerate the device
//correctly if the USB device does not stay detached for a minimum amount of
//time before re-attaching to the USB bus.  For reliable operation, the USB
//device should stay detached for as long as a human would require to unplug and
//reattach a USB device (ex: 100ms+), to ensure the USB host software has a 
//chance to process the detach event and configure itself for a state ready for 
//a new attachment event.
void ResetDeviceCleanly(void)
{
    USBDisableWithLongDelay();
    Reset();    
    Nop();
    Nop();
}





//Routine used to write data to the flash memory from the ProgrammingBuffer[].
void WriteFlashBlock(void)      //Use to write blocks of data to flash.
{
    static unsigned char i;
    static unsigned char BytesTakenFromBuffer;
    static unsigned char CorrectionFactor;

    #ifdef __XC8__
        static ROM uint8_t* pROM;

        pROM = (ROM uint8_t*)(ProgrammedPointer - BufferedDataIndex);
        TBLPTRU = 0x00;
        TBLPTRH = (uint8_t)((uint16_t)pROM >> 8);
        TBLPTRL = (uint8_t)pROM;
    #else
        TBLPTR = (ProgrammedPointer - BufferedDataIndex);
    #endif

    BytesTakenFromBuffer = 0;

    //Check the lower 5 bits of the TBLPTR to verify it is pointing to a 32 byte aligned block (5 LSb = 00000).
    //If it isn't, need to somehow make it so before doing the actual loading of the programming latches.
    //In order to maximize programming speed, the PC application meant to be used with this firmware will not send 
    //large blocks of 0xFF bytes.  If the PC application
    //detects a large block of unprogrammed space in the hex file (effectively = 0xFF), it will skip over that
    //section and will not send it to the firmware.  This works, because the firmware will have already done an
    //erase on that section of memory when it received the ERASE_DEVICE command from the PC.  Therefore, the section
    //can be left unprogrammed (after an erase the flash ends up = 0xFF).
    //This can result in a problem however, in that the next genuine non-0xFF section in the hex file may not start 
    //on a 32 byte aligned block boundary.  This needs to be handled with care since the microcontroller can only 
    //program 32 byte blocks that are aligned with 32 byte boundaries.
    //So, use the below code to avoid this potential issue.

    #if(WRITE_BLOCK_SIZE == 0x20)
        CorrectionFactor = (TBLPTRL & 0b00011111);  //Correctionfactor = number of bytes tblptr must go back to find the immediate preceeding 32 byte boundary
        TBLPTRL &= 0b11100000;                      //Move the table pointer back to the immediately preceeding 32 byte boundary
    #elif(WRITE_BLOCK_SIZE == 0x10)
        CorrectionFactor = (TBLPTRL & 0b00001111);  //Correctionfactor = number of bytes tblptr must go back to find the immediate preceeding 16 byte boundary
        TBLPTRL &= 0b11110000;                      //Move the table pointer back to the immediately preceeding 16 byte boundary
    #elif(WRITE_BLOCK_SIZE == 0x8)
        CorrectionFactor = (TBLPTRL & 0b00000111);  //Correctionfactor = number of bytes tblptr must go back to find the immediate preceeding 16 byte boundary
        TBLPTRL &= 0b11111000;                      //Move the table pointer back to the immediately preceeding 16 byte boundary
    #else
        #warning "Double click this error message and fix this section for your microcontroller type."
    #endif

    for(i = 0; i < WRITE_BLOCK_SIZE; i++)   //Load the programming latches
    {
        if(CorrectionFactor == 0)
        {
            if(BufferedDataIndex != 0)  //If the buffer isn't empty
            {
                TABLAT = ProgrammingBuffer[BytesTakenFromBuffer];
                #ifdef __XC8__
                    #asm
               			tblwtpostinc
                    #endasm
                #else //must be C18 instead
                    _asm tblwtpostinc _endasm
                #endif

                BytesTakenFromBuffer++;
                BufferedDataIndex--;    //Used up a byte from the buffer.
            }
            else    //No more data in buffer, need to write 0xFF to fill the rest of the programming latch locations
            {
                TABLAT = 0xFF;
                #ifdef __XC8__
                    #asm
                        tblwtpostinc
                    #endasm
                #else //must be C18 instead
                    _asm tblwtpostinc _endasm
                #endif


            }
        }
        else
        {
            TABLAT = 0xFF;
            #ifdef __XC8__
                #asm
                    tblwtpostinc
                #endasm
            #else //must be C18 instead
                _asm tblwtpostinc _endasm
            #endif
            CorrectionFactor--;
        }
    }

    //  TBLPTR--;               //Need to make table pointer point to the region which will be programmed before initiating the programming operation
    #ifdef __XC8__
        #asm
            tblrdpostdec
        #endasm
    #else //must be C18 instead
        _asm tblrdpostdec _endasm  //Do this instead of TBLPTR--; since it takes less code space.
    #endif

    EECON1 = 0b10100100;    //flash programming mode
    UnlockAndActivate(CORRECT_UNLOCK_KEY);

    //Now need to fix the ProgrammingBuffer[].  We may not have taken a full 32 bytes out of the buffer.  In this case,
    //the data is no longer justified correctly.
    for(i = 0; i < BufferedDataIndex; i++)  //Need to rejustify the remaining data to the "left" of the buffer (if there is any left)
    {
        ProgrammingBuffer[i] = ProgrammingBuffer[BytesTakenFromBuffer+i];
    }
}


void WriteConfigBits(void)  //Also used to write the Device ID
{
    static unsigned char i;

    #ifdef __XC8__
        TBLPTRU = 0x30;
        TBLPTRH = (uint8_t)((uint16_t)PacketFromPC.Address >> 8);
        TBLPTRH = (uint8_t)(PacketFromPC.Address);
    #else
        TBLPTR = (uint24_t)PacketFromPC.Address;
    #endif

    for(i = 0; i < PacketFromPC.Size; i++)
    {
        TABLAT = PacketFromPC.Data[i+(REQUEST_DATA_BLOCK_SIZE-PacketFromPC.Size)];
        #ifdef __XC8__
            #asm
                tblwt
            #endasm
        #else //must be C18 instead
            _asm tblwt _endasm
        #endif

        EECON1 = 0b11000100;    //Config bits programming mode
        UnlockAndActivate(CORRECT_UNLOCK_KEY);

        #ifdef __XC8__
            #asm
                tblrdpostinc
            #endasm
        #else //must be C18 instead
            _asm tblrdpostinc _endasm
        #endif
    }
}

#if defined(DEVICE_WITH_EEPROM)
void WriteEEPROM(void)
{
    static unsigned char i;
    
    for(i = 0; i < PacketFromPC.Size; i++)
    {
        EEADR = (((unsigned char)PacketFromPC.Address) + i);
        EEDATA = PacketFromPC.Data[i+(REQUEST_DATA_BLOCK_SIZE-PacketFromPC.Size)];

        EECON1 = 0b00000100;    //EEPROM Write mode
        UnlockAndActivate(CORRECT_UNLOCK_KEY);
    }

}
#endif

//It is preferrable to only place this sequence in only one place in the flash memory.
//This reduces the probabilty of the code getting executed inadvertently by
//errant code.  It is also recommended to enable BOR (in hardware) and/or add
//software checks to avoid microcontroller "overclocking".  Always make sure
//to obey the voltage versus frequency graph in the datasheet, even during
//momentary events (such as the power up and power down ramp of the microcontroller).
void UnlockAndActivate(unsigned char UnlockKey)
{
    INTCONbits.GIE = 0;     //Make certain interrupts disabled for unlock process.

    //Make sure voltage is sufficient for safe self erase/write operations
    LowVoltageCheck();

    //Check to make sure the caller really was trying to call this function.
    //If they were, they should always pass us the CORRECT_UNLOCK_KEY.
    if(UnlockKey != CORRECT_UNLOCK_KEY)
    {
        //Warning!  Errant code execution detected.  Somehow this 
        //UnlockAndActivate() function got called by someone that wasn't trying
        //to actually perform an NVM erase or write.  This could happen due to
        //microcontroller overclocking (or undervolting for an otherwise allowed
        //CPU frequency), or due to buggy code (ex: incorrect use of function 
        //pointers, etc.).  In either case, we should execute some fail safe 
        //code here to prevent corruption of the NVM contents.
        OSCCON = 0x03;  //Switch to INTOSC at low frequency
        while(1)
        {
            Sleep();
        }    
        Reset();
    }    
    
    #ifdef __XC8__
        EECON2 = 0x55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1;
    #else
        _asm
        //Now unlock sequence to set WR (make sure interrupts are disabled before executing this)
        MOVLW 0x55
        MOVWF EECON2, 0
        MOVLW 0xAA
        MOVWF EECON2, 0
        BSF EECON1, 1, 0        //Performs write by setting WR bit
        _endasm
    #endif

    while(EECON1bits.WR);   //Wait until complete (relevant when programming EEPROM, not important when programming flash since processor stalls during flash program)  
    EECON1bits.WREN = 0;    //Good practice now to clear the WREN bit, as further protection against any accidental activation of self write/erase operations.
}   


//Note: The ClrWdt() and "_asm tblrdpostinc _endasm" are inline assembly language
//instructions.  The ClearWatchdog() and  TableReadPostIncrement() functions are 
//theoretically extraneous, since the operations being accomplished could be
//done without calling them as separate functions.  However, when using inline
//assembly language, the C18 compiler normally doesn't know what the code will
//actually do (ex: will it modify STATUS reg, WREG, BSR contents??).  As a 
//result, it is potentially dangerous for the C compiler to make assumptions,
//that might turn out not to be correct.  Therefore, the C18 compiler disables
//the compiler optimizations for a function, when one or more inline asm 
//instructions are located within the C function.  Therefore, to promote best
//code size optimizations from the C18 compiler, it is best to locate inline
//assembly sequences in their own separate C functions, that do not contain much
//other code (which could otherwise be optimized by the C compiler).  This often
//results in the smallest code size, and is the reason it is being done here.
void TableReadPostIncrement(void)
{
    #ifdef __XC8__
        #asm
            tblrdpostinc
        #endasm
    #else //must be C18 instead
        _asm tblrdpostinc _endasm
    #endif
}    

/** EOF BootPIC18NonJ.c *********************************************************/
