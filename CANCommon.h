/* File:         CANCommon.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for Common Mode CAN Communication
 * using the Hindsight CAN Communication standard.
 */

#pragma once

#include "CANPacket.h"

void AssembleEmergencyStopPacket(CANPacket *packet,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerialNumber,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerialNumber,
    uint8_t errorCode);
void AssembleGroupBroadcastingEmergencyStopPacket(CANPacket *packet, 
    uint8_t groupCode, 
    uint8_t senderDeviceGroup, 
    uint8_t senderDeviceSerialNumber, 
    uint8_t errorCode);
void AssembleBrodcastEmergencyStopPacket(CANPacket *packet, 
    uint8_t senderDeviceGroup, 
    uint8_t senderDeviceSerialNumber, 
    uint8_t errorCode);
uint8_t GetEmergencyStopErrorCode(CANPacket *packet);

uint32_t GetTimeBetweenHeartbeatPacket(CANPacket *packet, uint32_t lastHeartbeat);
uint32_t GetHeartbeatTimeStamp(CANPacket *packet);
void AssembleHeartbeatPacket(CANPacket *packetToAssemble, 
    int broadcast, 
    uint8_t senderSerial,
    uint8_t senderDeviceGroup,
    uint8_t heartbeatLeniencyCode,
    uint32_t timestamp);

void AssembleOverrideProtectionPacket(CANPacket *packetToAssemble, uint8_t targetGroup, uint8_t targetSerial);