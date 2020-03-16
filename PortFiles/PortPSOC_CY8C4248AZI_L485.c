/*
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 * 
 * Port specific notes:
 * Design must include a CAN block called CAN (not CAN0 or variation)
 * Must auto generate code within PSoC IDE to use APIs
 */
#if CHIP_TYPE == CHIP_TYPE_PSOC_CY8C4248AZI_L485//Replace this with the chip you are porting

#include "../Port.h"
//#include "/home/dylan-wsl/Documents/HRT/Motor-Unit-Firmware-A-2019-2020/Generated_Source/PSoC4/CAN/CAN.h"
//Use include below for code, above is just for development
#include "../../Generated_Source/PSoC4/CAN.h"

//Flag internal to this port, 0xFF if no message waiting, doubles as mailbox number
volatile uint8_t messagePresentFlag = 0xFF;
CANPacket lastestMessage;//internal to this port

#define STATUS_MAILBOX0 0x1
#define STATUS_MAILBOX1 0x2
#define STATUS_MAILBOX2 0x4
#define STATUS_MAILBOX3 0x8
#define STATUS_MAILBOX4 0x10
#define STATUS_MAILBOX5 0x20
CY_ISR(CAN_FLAG_ISR)
{
    if(!(~messagePresentFlag)) //Skip handling interrupt if message has not been handled by loop
    //TODO: will it immediately retrigger the ISR without giving the loop anytime to run?
    {
     /* Clear Receive Message Register flag */
//    CAN_INT_SR_REG = CAN_RX_MESSAGE_MASK;
    
    uint32_t statusReg = (uint32_t) CAN_BUF_SR_REG;
    //Hardcoded for speed, translation from reg
    switch(statusReg)
    {
        case STATUS_MAILBOX0:
            messagePresentFlag = 0;
            break;
        case STATUS_MAILBOX1:
            messagePresentFlag = 1;
            break;
        case STATUS_MAILBOX2:
            messagePresentFlag = 2;
            break;
        case STATUS_MAILBOX3:
            messagePresentFlag = 3;
            break;
        case STATUS_MAILBOX4:
            messagePresentFlag = 4;
            break;
        case STATUS_MAILBOX5:
            messagePresentFlag = 5;
            break;
    }

    lastestMessage.id = CAN_GET_RX_ID(messagePresentFlag);
    lastestMessage.dlc = CAN_GET_DLC(messagePresentFlag);
    lastestMessage.data[0] = CAN_RX_DATA_BYTE1(messagePresentFlag);
    lastestMessage.data[1] = CAN_RX_DATA_BYTE2(messagePresentFlag);
    lastestMessage.data[2] = CAN_RX_DATA_BYTE3(messagePresentFlag);
    lastestMessage.data[3] = CAN_RX_DATA_BYTE4(messagePresentFlag);
    lastestMessage.data[4] = CAN_RX_DATA_BYTE5(messagePresentFlag);
    lastestMessage.data[5] = CAN_RX_DATA_BYTE6(messagePresentFlag);
    lastestMessage.data[6] = CAN_RX_DATA_BYTE7(messagePresentFlag);
    lastestMessage.data[7] = CAN_RX_DATA_BYTE8(messagePresentFlag);
    }
}

int deviceAddress;
int deviceGroup;
void InitCAN(int deviceGroupInput, int deviceAddressInput)
{
    CAN_Start();
    //TODO: I'm sure there's a better way of doing this part
    deviceGroup = deviceGroupInput;
    deviceAddress = deviceAddressInput;

    /* Initial values of CAN RX and TX registers 
    Copy and pasted from PSoC autogened code, so not sure what all the 
    values in rxcmd mean.  0x48u for linked registers (queues up), may experiment later with this.
    Link to TRM description on slite*/
    #define ID_BITS_NORMAL_PRIO 0x80000000u
    #define ID_BITS_HIGH_PRIO   0x00000000u
    const CAN_RX_CFG CYCODE CAN_RXConfigStruct[] =
    {
        { 0u, 0x28u, 0x1FFFF9u, ID_BITS_NORMAL_PRIO },//Broadcast message
        { 1u, 0x28u, 0x1FFFF9u, ID_BITS_NORMAL_PRIO | (deviceGroup << 26)},//Group broadcast
        { 2u, 0x28u, 0x1FFFF9u, ID_BITS_NORMAL_PRIO | (deviceGroup << 26) | (deviceAddress << 22) },//Direct Message
        { 3u, 0x28u, 0x1FFFF9u, ID_BITS_HIGH_PRIO },
        { 4u, 0x28u, 0x1FFFF9u, ID_BITS_HIGH_PRIO | (deviceGroup << 26)},
        { 5u, 0x28u, 0x1FFFF9u, ID_BITS_HIGH_PRIO | (deviceGroup << 26) | (deviceAddress << 22) },
        { 6u, 0x0u, 0x0u, 0x0u },
        { 7u, 0x0u, 0x0u, 0x0u },
        { 8u, 0x0u, 0x0u, 0x0u },
        { 9u, 0x0u, 0x0u, 0x0u },
        { 10u, 0x0u, 0x0u, 0x0u },
        { 11u, 0x0u, 0x0u, 0x0u },
        { 12u, 0x0u, 0x0u, 0x0u },
        { 13u, 0x0u, 0x0u, 0x0u },
        { 14u, 0x0u, 0x0u, 0x0u },
        { 15u, 0x0u, 0x0u, 0x0u },
    };
    // Implement hardware initialization of CAN
    // Including receive filter    /* Initialize TX mailboxes */
    
    /* More copy and pasted autogen code*/

        static const CAN_TX_CFG CYCODE CAN_TXConfigStruct[] =
    {
        { 0u, 0x880008u, 0x0u },
        { 1u, 0x880008u, 0x0u },
        { 2u, 0x880008u, 0x0u },
        { 3u, 0x880008u, 0x0u },
        { 4u, 0x880008u, 0x0u },
        { 5u, 0x880008u, 0x0u },
        { 6u, 0x880008u, 0x0u },
        { 7u, 0x880008u, 0x0u }
    };

    uint8 result = CYRET_SUCCESS;
    uint8 i;


    for (i = 0u; i < CAN_NUMBER_OF_TX_MAILBOXES; i++)
    {
        if (CAN_TxBufConfig((const CAN_TX_CFG *)
            (&CAN_TXConfigStruct[i])) != CYRET_SUCCESS)
        {
            result = CAN_FAIL;
            break;
        }
    }

    if (result == CYRET_SUCCESS)
    {
        /* Initialize RX mailboxes */
        for (i = 0u; i < CAN_NUMBER_OF_RX_MAILBOXES; i++)
        {
            if (CAN_RxBufConfig((const CAN_RX_CFG *)
                (&CAN_RXConfigStruct[i])) != CYRET_SUCCESS)
            {
                result = CAN_FAIL;
                break;
            }
        }
    }

    CY_ISR_PROTO(CAN_FLAG_ISR);
}
int SendCANPacket(CANPacket *packetToSend)
{
    if(!packetToSend) {return ERROR_NULL_POINTER;}
    CAN_TX_MSG PSoCMessage;
    CAN_DATA_BYTES_MSG PSoCData;
    PSoCMessage.id = packetToSend->id;
    PSoCMessage.rtr = 0x0;
    PSoCMessage.ide = 0x0;//Not extended
    PSoCMessage.dlc = packetToSend->dlc;
    PSoCMessage.irq = 0x0;
    PSoCMessage.msg = &PSoCData;
    
    memcpy(PSoCData.byte, packetToSend->data, 8);

    if(CAN_SendMsg(&PSoCMessage) == CYRET_SUCCESS) {
        return ERROR_NONE;
    } else
    {
        return ERROR_GENERIC_ERROR;
    }
}
int PollAndReceiveCANPacket(CANPacket *receivedPacket)
{
    if(!receivedPacket) {return ERROR_NULL_POINTER;}
    if(~messagePresentFlag)
    {
        *(receivedPacket) = lastestMessage;
        messagePresentFlag = 0xFF; //No message present
        return ERROR_NONE;
    }
    return 0x02; //No message received error
}

uint8_t getLocalDeviceSerial()
{
    //Reading DIP switches? Hard coded?
    //This might be board specific, rather than chip specific.
    return deviceAddress; // example value (also used for testing)
}
uint8_t getLocalDeviceGroup()
{
    //Definitely board specific.
    return deviceGroup; // example value (also used for testing)
}

uint8_t getChipType()
{
    return CHIP_TYPE; 
    //Should be same for all ports, just not sure where to put it.
}
#endif //CHIP_TYPE == CHIP_TYPE_PSOC_CY8C4248AZI_L485
