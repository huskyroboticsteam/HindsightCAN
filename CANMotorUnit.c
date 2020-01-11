/* File:         CANMotorUnit.c
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction definitions for CAN Packet manipulation
 * using the Hindsight CAN Communication standard. Specific files
 * for the motor unit boards.
 */

#include "CANMotorUnit.h"

void AssembleModeSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t mode)
{
    
}

uint8_t GetModeFromPacket(CANPacket *packet)
{

}

void AssembleChipTypePullPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t yourChipType)
{

}

void AssembleChipTypeReportPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t chipType)
{

}

uint8_t GetChipTypeFromPacket(CANPacket *packet)
{

}

void AssemblePWMDirSetPacket(CANPacket *packetToAssemble, 
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t PWMSet)
{

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
