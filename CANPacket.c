#include "CANPacket.h"

// Contains functons for creating CAN packets
// Functions here will be used in Jetson (Rover.cpp) and electronics boards

// ----------- GENERAL PURPOSE Functions ------------------------------ //

// Constructs a CAN ID according to standards set by electronics subsystem
// for PY2020 rover. Not compatible with Orpheus (PY2019)
// Inputs: 
//      priority:   A byte determing if the packet should be prioritized
//                  High priority would mean setting this value to 0
//                  Low priority would mean setting this value to 1
//      devGroup:   ID of device group
//      devSerial:  Serial value of device in the device group
// Output:
//      CANID:      CAN ID with correct formatting        
uint16_t ConstructCANID(uint8_t priority, uint8_t devGroup, uint8_t devSerial)
{
    uint16_t CANID = 0x0000;
    CANID = CANID | ((priority & 0x01) << 10);
    CANID = CANID | ((devGroup & 0x0F) << 6);
    CANID = CANID | (devSerial & 0x2F);

    return CANID;
}

// Creates a CANPacket that can be used by fuctions in this file
// Inputs:
//      id:         CAN ID for the packet
//      dlc:        Data length for the packet. It's the number of bytes used
//                  in data payload for packet
//      data:       An array of bytes used for sending data over CAN
// Outputs:
//      CANPacket:  A struct used for storing the parts needed for a CAN Packet
CANPacket ConstructCANPacket(uint16_t id, uint8_t dlc, uint8_t* data)
{
    CANPacket cp;
    cp.id = id;
    cp.dlc = dlc;   
    for(int i = 0; i < dlc; i++)
    {
        cp.data[i] = data[i];
    }

    return cp;
}

// Gets the device serial number from CAN packet
// Inputs:
//      packet:     CAN Packet to analyze
// Outputs:
//                  A byte representing the device
//                  serial number.
uint8_t GetDeviceSerialNumber(CANPacket *packet)
{
    uint8_t id = (packet->id & 0x00FF);
    // Strip fo only serial number portion of id
    return id & 0x3F;
}

// Returns Sender device serial number as 
//      packet:       CAN packet from which to resolve serial number
// Outputs:
//                  A byte representing the sender device number
uint8_t GetSenderDeviceSerialNumber(CANPacket *packet)
{
    return ((packet->data[0] & 0xC0) >> 4) + ((packet->data[1] & 0xC0) >> 6);
}

// Gets the device group code from CAN packet
// Inputs:
//      packet:     CAN Packet to analyze
// Outputs:
//                  A byte representing the device
//                  group code.
uint8_t GetDeviceGroupCode(CANPacket *packet)
{
    uint8_t group = 0;
    int id = packet->id;
    id = id & 0x07F; // Strip to ID-only portion of ID
    group = (uint8_t) ((id & 0x03C0) >> 6);
    return group;
}

// Gets the sender device group number from the payload data
// Inputs:
//      data:       Address of the byte array of the payload from CAN packet
// Outputs:
//                  A byte representing the sender device number
uint8_t GetSenderDeviceGroupCode(CANPacket *packet)
{
    return (packet->data[1] & 0x2F);
}

// Ensures that the given packet is of a specified group
// Inputs:
//      packet:         CAN Packet to check
//      expectedType:   ExpectedType of CAN packet
// Outputs:
//                  0 if packet not of expectedType,
//                  Other int otherwise
int PacketIsInGroup(CANPacket *packet, uint8_t expectedType) 
{
    return GetDeviceGroupCode(packet) == expectedType;
}

int SenderPacketIsInGroup(CANPacket *packet, uint8_t expectedType)
{
    return GetSenderDeviceGroupCode(packet) == expectedType;
}

int GetPacketID(CANPacket *packet)
{
    return packet->data[0] & 0x3F;
}

int PacketIsOfID(CANPacket *packet, uint8_t expectedID)
{
    return GetPacketID(packet) == expectedID;
}

// Determines if a given packet targets a specific device
// Useful for determing if a packet should be interpreted by
// the device
// Inputs:
//      packet:                     CAN Packet to check
//      targetDeviceGroup:          Device group of target device
//      targetDeviceSerialNumber:   Serial number of target device
// Outputs:
//                  Returns 0 if packet does not target device
//                  Returns any other int if packet does
int TargetsDevice(CANPacket *packet, uint8_t targetDeviceGroup, uint8_t targetDeviceSerialNumber)
{
    uint8_t packetGroup = GetDeviceGroupCode(packet);
    if (packetGroup == targetDeviceGroup) 
    {
        uint8_t serialNumber = GetDeviceSerialNumber(packet);
        // Return if serial number matches target
        if (serialNumber == targetDeviceSerialNumber) { return 1; }
        // Otherwise only return true if packet is broadcast to group
        return serialNumber == DEVICE_SERIAL_BROADCAST;
    }
    // Otherwise only return true if packet is broadcast to all devices
    return packetGroup == DEVICE_GROUP_BROADCAST;
}

// ---------------- COMMON INTERFACING FUNCTIONS ------------------- //

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
//      packet:                 CAN Packet to assemble (will overwrite).
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
    packetToAssemble->data[0] = ((senderDeviceGroup & 0x0C) << 6) | ID_HEARTBEAT;
    packetToAssemble->data[1] = ((senderDeviceGroup & 0x03) << 6) | senderSerial;
    packetToAssemble->data[2] = heartbeatLeniencyCode;
    packetToAssemble->data[3] = (timestamp & 0xFF000000) >> 24;
    packetToAssemble->data[4] = (timestamp & 0x00FF0000) >> 16;
    packetToAssemble->data[5] = (timestamp & 0x0000FF00) >> 8;
    packetToAssemble->data[6] = (timestamp & 0x000000FF);
}