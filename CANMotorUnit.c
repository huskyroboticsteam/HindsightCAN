/* File:         CANMotorUnit.c
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction definitions for CAN Packet manipulation
 * using the Hindsight CAN Communication standard. Specific files
 * for the motor unit boards.
 */

#include "CANPacket.h"
#include "CANMotorUnit.h"

void AssembleModeSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t mode)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_MODE_SEL;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_MODE_SEL);
    packetToAssemble->data[nextByte] = mode;
}

uint8_t GetModeFromPacket(CANPacket *packet)
{
    return packet->data[1];
}

void AssembleChipTypePullPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t yourChipType)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_CHIP_TYPE_PULL;
    int nextByte = WriteSenderSerialAndPacketID(packetToAssemble->data, senderDeviceGroup, senderDeviceSerial, ID_MOTOR_UNIT_CHIP_TYPE_PULL);
    packetToAssemble->data[nextByte] = yourChipType;
}

void AssembleChipTypeReportPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t chipType)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_CHIP_TYPE_REP;
    int nextByte = WriteSenderSerialAndPacketID(packetToAssemble->data, senderDeviceGroup, senderDeviceSerial, ID_MOTOR_UNIT_CHIP_TYPE_REP);
    packetToAssemble->data[nextByte] = chipType;
}

uint8_t GetChipTypeFromPacket(CANPacket *packet)
{

}

void AssemblePWMDirSetPacket(CANPacket *packetToAssemble, 
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t PWMSet)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_PWM_DIR_SET;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, DLC_MOTOR_UNIT_PWM_DIR_SET);
    PackIntIntoDataMSBFirst(packetToAssemble, PWMSet, nextByte);
}

int32_t GetPWMFromPacket(CANPacket *packet)
{

}

int32_t GetDirectionFromPacket(CANPacket *packet)
{

}

void AssemblePIDTargetSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t target)
{

}

uint8_t GetPIDTargetFromPacket(CANPacket *packet)
{

}

void AssemblePSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t pCoef)
{

}

int32_t GetPFromPacket(CANPacket *packet)
{

}

void AssembleISetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t iCoef)
{

}

int32_t GetIFromPacket(CANPacket *packet)
{

}

void AssembleDSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t dCoef)
{

}

int32_t GetDFromPacket(CANPacket *packet)
{

}

void AssembleInitializePacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial)
{

}

void AssembleLimitSwitchAlertPacket(CANPacket *packetToAssemble)
{

}

void AssembleEncoderPPJRSetPacket(CANPacket *packetToAssemble)
{

}

uint32_t GetEncoderPPJRFromPacket(CANPacket *packet)
{

}

void AssembleMaxJointRevolutionPacket(CANPacket *packetToAssemble)
{

}

uint32_t GetMaxJointRevolutionsFromPacket(CANPacket *packet)
{

}
