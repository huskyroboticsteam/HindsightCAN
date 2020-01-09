/* File:         CANCommon.c
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction definitions for Common Mode CAN Communication
 * using the Hindsight CAN Communication standard.
 */

#include "CANPacket.h"
#include "CANCommon.h"

// Assembles Emergency Stop Packet with given parameters
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      targetDeviceGroup:          Group to target
//      targetDeviceSerialNumber:   Serial number of target device
//      senderDeviceGroup:          Device group of sender device
//      senderDeviceSerialNumber:   Device serial number of sender device
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleEmergencyStopPacket(CANPacket *packet,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerialNumber,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerialNumber,
    uint8_t errorCode)
{
    packet->dlc = 3;
    uint16_t id = ConstructCANID(PACKET_PRIORITY_HIGH, targetDeviceGroup, targetDeviceSerialNumber);
    WriteSenderSerialAndPacketID(packet->data, senderDeviceGroup, senderDeviceSerialNumber, ID_ESTOP);
    packet->data[2] = errorCode;
}

// Assembles Emergency Stop Packet with given parameters.
// This will broadcast the emergency stop command to a desired device group.
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      deviceGroup:                Group to target
//      senderDeviceGroup:          Device group of sender device
//      senderDeviceSerialNumber:   Device serial number of sender device
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleGroupBroadcastingEmergencyStopPacket(CANPacket *packet, 
    uint8_t groupCode, 
    uint8_t senderDeviceGroup, 
    uint8_t senderDeviceSerialNumber, 
    uint8_t errorCode)
{
    AssembleEmergencyStopPacket(packet, groupCode, DEVICE_SERIAL_BROADCAST, senderDeviceGroup, senderDeviceSerialNumber, errorCode);
}

// Assembles Emergency Stop Packet with given parameters.
// This will broadcast the emergency stop command to all devices
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      senderDeviceGroup:          Device group of sender device
//      senderDeviceSerialNumber:   Device serial number of sender device
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleBrodcastEmergencyStopPacket(CANPacket *packet, 
    uint8_t senderDeviceGroup, 
    uint8_t senderDeviceSerialNumber, 
    uint8_t errorCode)
{
    AssembleGroupBroadcastingEmergencyStopPacket(packet, DEVICE_GROUP_BROADCAST, senderDeviceGroup, senderDeviceSerialNumber, errorCode);
}

// Gets the Error Code reported from an emergency stop packet.
// Inputs:
//      packet:     Packet to check.
uint8_t GetEmergencyStopErrorCode(CANPacket *packet)
{
    if (PacketIsOfID(packet, ID_ESTOP))
    {
        return packet->data[2];
    }
    else { return -1; }
}

// Validates the Heartbeat Packet, returns time between previous Heartbeat packets
// Inputs:
//      packet:         CAN Packet to check
//      lastHeartbeat:  Timestamp (ms) of last detected heartbeat
// Outputs:
//                  Time (in ms) between this heartbeat and the previous detected heartbeat
//                  Negative value if packet is not valid heartbeat packet
uint32_t GetTimeBetweenHeartbeatPacket(CANPacket *packet, uint32_t lastHeartbeat)
{
    if (PacketIsOfID(packet, ID_HEARTBEAT)) 
    {
        return GetHeartbeatTimeStamp(packet) - lastHeartbeat;
    }
    else { return -1; }
}

// Validates the Heartbeat Packet, returns time between previous Heartbeat packets
// Inputs:
//      packet:         CAN Packet to check
//      lastHeartbeat:  Timestamp (ms) of last detected heartbeat
// Outputs:
//                  Time (in ms) of the timestamp within the packet
//                  Default return value is uint32 max value, which is used
//                  if the packet is corrupt or not a heartbeat packet.
uint32_t GetHeartbeatTimeStamp(CANPacket *packet)
{
    if (PacketIsOfID(packet, ID_HEARTBEAT)) 
    {
        uint32_t time = (packet->data[3] << 24);
        time |= (packet->data[4] << 16);
        time |= (packet->data[5] << 8);
        time |= packet->data[6]; 
        return time;
    }
    else { return -1; }
}

// Validates the Heartbeat Packet, returns the heartbeat leniency code of the packet
// Inputs:
//      packet:     CAN Packet to check
// Outputs:
//                  Heartbeat leniency code of given packet
uint8_t GetHeartbeatLeniencyCode(CANPacket *packet)
{
    if (PacketIsOfID(packet, ID_HEARTBEAT))
    {
        return packet->data[2];
    } else {
        return 0x00;
    }
}

// Assembles Heartbeat Packet with given parameters
// Inputs:
//      packetToAssemble:       CAN Packet to assemble (will overwrite).
//      broadcast:              1 if broadcast to all devices. 0 to return to MAIN_CPU / Jetson.
//      senderSerial:           Serial number of sender device
//      senderDeviceGroup:      Device group of sender device
//      heartbeatLeniencyCode:  Max time between heartbeats before system automatically enters a safe operating condition.
//      timestamp:              Current timestamp as seen by the sender device. (ms)
void AssembleHeartbeatPacket(CANPacket *packetToAssemble, 
    int broadcast, 
    uint8_t senderSerial,
    uint8_t senderDeviceGroup,
    uint8_t heartbeatLeniencyCode,
    uint32_t timestamp)
{
    uint16_t id = ConstructCANID(PACKET_PRIORITY_HIGH, DEVICE_GROUP_BROADCAST, DEVICE_SERIAL_BROADCAST);
    if (!broadcast)
    { 
        id = ConstructCANID(PACKET_PRIORITY_HIGH, DEVICE_GROUP_JETSON, DEVICE_SERIAL_JETSON);
    }
    uint8_t dlc = 0x07;

    packetToAssemble->id = id;
    packetToAssemble->dlc = dlc;
    WriteSenderSerialAndPacketID(packetToAssemble->data, senderDeviceGroup, senderSerial, ID_HEARTBEAT);
    packetToAssemble->data[2] = heartbeatLeniencyCode;
    packetToAssemble->data[3] = (timestamp & 0xFF000000) >> 24;
    packetToAssemble->data[4] = (timestamp & 0x00FF0000) >> 16;
    packetToAssemble->data[5] = (timestamp & 0x0000FF00) >> 8;
    packetToAssemble->data[6] = (timestamp & 0x000000FF);
}

// Assembles override protection packet with given parameters
// Inputs:
//      packetToAssemble:       CAN Packet to assemble (will overwrite).
//      targetGroup:            Device gorup of target device.
//      targetSerial:           Device serial of target device.
void AssembleOverrideProtectionPacket(CANPacket *packetToAssemble, uint8_t targetGroup, uint8_t targetSerial)
{
    uint16_t id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->id = id;
    packetToAssemble->dlc = 1;
    WritePacketIDOnly(packetToAssemble->data, ID_OVRD_PROTECTION);
}