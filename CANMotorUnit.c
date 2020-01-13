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
    packetToAssemble->id = ConstructCANID(PRIO_MOTOR_UNIT_MODE_SEL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_MODE_SEL;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_MODE_SEL);
    packetToAssemble->data[nextByte] = mode;
}

uint8_t GetModeFromPacket(CANPacket *packet)
{
    return packet->data[1];
}


void AssemblePWMDirSetPacket(CANPacket *packetToAssemble, 
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t PWMSet)
{
    packetToAssemble->id = ConstructCANID(PRIO_MOTOR_UNIT_PWM_DIR_SET, targetDeviceGroup, targetDeviceSerial);
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
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_PID_POS_TGT_SET;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_PID_POS_TGT_SET);
    PackIntIntoDataMSBFirst(packetToAssemble, target, nextByte);
}

uint8_t GetPIDTargetFromPacket(CANPacket *packet)
{
    return DecodeBytesToIntMSBFirst(packet->data, DLC_MOTOR_UNIT_PID_POS_TGT_SET - 5, DLC_MOTOR_UNIT_PID_POS_TGT_SET - 1);
}

void AssemblePSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t pCoef)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_PID_P_SET;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_PID_P_SET);
    PackIntIntoDataMSBFirst(packetToAssemble, pCoef, nextByte);
}

int32_t GetPFromPacket(CANPacket *packet)
{
    return DecodeBytesToIntMSBFirst(packet->data, DLC_MOTOR_UNIT_PID_P_SET - 5, DLC_MOTOR_UNIT_PID_P_SET - 1);
}

void AssembleISetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t iCoef)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_PID_I_SET;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_PID_I_SET);
    PackIntIntoDataMSBFirst(packetToAssemble, iCoef, nextByte);
}

int32_t GetIFromPacket(CANPacket *packet)
{
    return DecodeBytesToIntMSBFirst(packet->data, DLC_MOTOR_UNIT_PID_I_SET - 5, DLC_MOTOR_UNIT_PID_I_SET - 1);
}

void AssembleDSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t dCoef)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_PID_D_SET;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_PID_D_SET);
    PackIntIntoDataMSBFirst(packetToAssemble, dCoef, nextByte);
}

int32_t GetDFromPacket(CANPacket *packet)
{
    return DecodeBytesToIntMSBFirst(packet->data, DLC_MOTOR_UNIT_PID_D_SET - 5, DLC_MOTOR_UNIT_PID_D_SET - 1);
}

void AssembleInitializePacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t initMode)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_MOTOR_UNIT_INIT;
    int nextByte = WritePacketIDOnly(packetToAssemble->data, ID_MOTOR_UNIT_INIT);
    packetToAssemble->data[nextByte] = initMode;
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
