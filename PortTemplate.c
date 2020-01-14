#if CHIP_TYPE == CHIP_TYPE_TEMPLATE//Replace this with the chip you are porting

#include "port.h"

int SendCANPacket(CANPacket *packetToSend)
{
    //Implement sending/ queing to send packet
}
CANPacket ReceiveCANPacket()
{
    //Implement get next CAN packet from buffers/registers
}

uint8_t getLocalDeviceSerial()
{
    //Reading DIP switches? Hard coded?
    //This might be board specific, rather than chip specific.
}
uint8_t getLocalDeviceGroup()
{
    //Definitely board specific.
}

uint8_t getChipType()
{
    return CHIP_TYPE; 
    //Should be same for all ports, just not sure where to put it.
}
#endif //CHIP_TYPE == CHIP_TYPE_TEMPLATE
