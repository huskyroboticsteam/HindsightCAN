/* File:         CANCommon.c
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction definitions for Common Mode CAN Communication
 * using the Hindsight CAN Communication standard.
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 * 
 * Modified to include packet assembly, decoding, RTR support, and error handling for ODrive CANSimple protocol.
 */

#include "CANPacket.h"
#include "CANCommon.h"
#include "Port.h"

// Assembles Emergency Stop Packet with given parameters
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      targetDeviceGroup:          Group to target
//      targetDeviceSerialNumber:   Serial number of target device
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleEmergencyStopPacket(CANPacket *packet,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerialNumber,
    uint8_t errorCode)
{
    packet->dlc = DLC_ESTOP;
    packet->id = ConstructCANID(PACKET_PRIORITY_HIGH, targetDeviceGroup, targetDeviceSerialNumber);
    packet->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_ESTOP);
    packet->data[nextByte] = errorCode;
}

// Assembles Emergency Stop Packet with given parameters.
// This will broadcast the emergency stop command to a desired device group.
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      deviceGroup:                Group to target
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleGroupBroadcastingEmergencyStopPacket(CANPacket *packet, 
    uint8_t groupCode, 
    uint8_t errorCode)
{
    AssembleEmergencyStopPacket(packet, groupCode, DEVICE_SERIAL_BROADCAST, errorCode);
}

// Assembles Emergency Stop Packet with given parameters.
// This will broadcast the emergency stop command to all devices
// Inputs:
//      packet:                     CAN Packet to assemble (will overwrite).
//      errorCode:                  Emergency stop error code. E.G. ESTOP_ERR_GENERAL
void AssembleBrodcastEmergencyStopPacket(CANPacket *packet, 
    uint8_t errorCode)
{
    AssembleGroupBroadcastingEmergencyStopPacket(packet, DEVICE_GROUP_BROADCAST, errorCode);
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
        uint32_t time = ((uint32_t)packet->data[3] << 24);
        time |= ((uint32_t)packet->data[4] << 16);
        time |= ((uint32_t)packet->data[5] << 8);
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
//      heartbeatLeniencyCode:  Max time between heartbeats before system automatically enters a safe operating condition.
//      timestamp:              Current timestamp as seen by the sender device. (ms)
//TODO, upon approval from @jaden, delete senderGroup and senderSerial params, as these are handled by getLocal functs
void AssembleHeartbeatPacket(CANPacket *packetToAssemble, 
    int broadcast, 
    uint8_t heartbeatLeniencyCode,
    uint32_t timestamp)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_HIGH, DEVICE_GROUP_BROADCAST, DEVICE_SERIAL_BROADCAST);
    if (!broadcast)
    { 
        packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_HIGH, DEVICE_GROUP_JETSON, DEVICE_SERIAL_JETSON);
    }
    packetToAssemble->dlc = DLC_HEARTBEAT;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packetToAssemble->data, ID_HEARTBEAT);
    packetToAssemble->data[nextByte] = heartbeatLeniencyCode;
    PackIntIntoDataMSBFirst(packetToAssemble->data, timestamp, nextByte + 1);
}

void AssembleFailReportPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t failedPacketID)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_FAIL_REPORT;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packetToAssemble->data, ID_FAIL_REPORT);
    packetToAssemble->data[nextByte] = failedPacketID;
}

// Assembles override protection packet with given parameters
// Inputs:
//      packetToAssemble:       CAN Packet to assemble (will overwrite).
//      targetGroup:            Device gorup of target device.
//      targetSerial:           Device serial of target device.
void AssembleOverrideProtectionPacket(CANPacket *packetToAssemble, uint8_t targetGroup, uint8_t targetSerial)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_OVRD_PROTECTION;
    packetToAssemble->rtr = 0;
    WritePacketIDOnly(packetToAssemble->data, ID_OVRD_PROTECTION);
}

void AssembleChipTypePullPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_CHIP_TYPE_PULL;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_CHIP_TYPE_PULL);
    packetToAssemble->data[nextByte] = getChipType();
}

void AssembleChipTypeReportPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetDeviceGroup, targetDeviceSerial);
    packetToAssemble->dlc = DLC_CHIP_TYPE_REP;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_CHIP_TYPE_REP);
    packetToAssemble->data[nextByte] = getChipType();
}

uint8_t GetChipTypeFromPacket(CANPacket *packet)
{
    return packet->data[1];
}

void AssembleTelemetryTimingPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial, 
    uint8_t telemetryTypeCode,
    uint32_t msBetweenReports)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_TELEMETRY_TIMING;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_TELEMETRY_TIMING);
    packetToAssemble->data[nextByte] = telemetryTypeCode;
    PackIntIntoDataMSBFirst(packetToAssemble->data, msBetweenReports, nextByte + 1);
}
uint32_t GetTelemetryTimingFromPacket(CANPacket *packetToAssemble)
{
    return DecodeTelemetryDataUnsigned(packetToAssemble);
}

void AssembleTelemetryPullPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial, 
    uint8_t telemetryTypeCode)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_TELEMETRY_PULL;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_TELEMETRY_PULL);
    packetToAssemble->data[nextByte] = telemetryTypeCode;
}

void AssembleTelemetryReportPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t telemetryTypeCode,
    int32_t data)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_TELEMETRY_REPORT;
    packetToAssemble->rtr = 0;
    int nextByte = WriteSenderSerialAndPacketID(packet->data, ID_TELEMETRY_REPORT);
    packetToAssemble->data[nextByte] = telemetryTypeCode;
    PackIntIntoDataMSBFirst(packet->data, data, nextByte + 1);
}

int32_t DecodeTelemetryDataSigned(CANPacket *packet)
{
    return DecodeBytesToIntMSBFirst(packet->data, 4, 8);
}

uint32_t DecodeTelemetryDataUnsigned(CANPacket *packet)
{
    return (uint32_t) DecodeTelemetryDataSigned(packet);
}

uint8_t DecodeTelemetryType(CANPacket *packet)
{
    return packet->data[3];
}

void AssembleRGBColorPacket(CANPacket *packetToAssemble,
    uint8_t targetGroup,
    uint8_t targetSerial,
    uint8_t addrLED,
    uint8_t R,
    uint8_t G,
    uint8_t B)
{
    packetToAssemble->id = ConstructCANID(PACKET_PRIORITY_NORMAL, targetGroup, targetSerial);
    packetToAssemble->dlc = DLC_LED_COLOR;
    packetToAssemble->rtr = 0;
    int nextByte = WritePacketIDOnly(packet->data, ID_LED_COLOR);
    packetToAssemble->data[nextByte] = R;
    packetToAssemble->data[nextByte + 1] = G;
    packetToAssemble->data[nextByte + 2] = B;
    packetToAssemble->data[nextByte + 3] = addrLED;   
}

// Helper function to pack a 32-bit integer into the CAN packet payload (little-endian)
static void PackIntIntoDataLSBFirst(uint8_t *data, uint32_t value, int startIndex)
{
    data[startIndex]     = (value & 0x000000FF);
    data[startIndex + 1] = (value & 0x0000FF00) >> 8;
    data[startIndex + 2] = (value & 0x00FF0000) >> 16;
    data[startIndex + 3] = (value & 0xFF000000) >> 24;
}

// Helper function to pack a float into the CAN packet payload as a 4-byte integer (little-endian)
static void PackFloatIntoDataLSBFirst(uint8_t *data, float value, int startIndex)
{
    union {
        float f;
        uint32_t u;
    } float_to_uint;
    float_to_uint.f = value;
    PackIntIntoDataLSBFirst(data, float_to_uint.u, startIndex);
}

// Helper function to decode a 32-bit integer from the CAN packet payload (little-endian)
static uint32_t DecodeIntFromDataLSBFirst(const uint8_t *data, int startIndex)
{
    return ((uint32_t)data[startIndex + 3] << 24) |
           ((uint32_t)data[startIndex + 2] << 16) |
           ((uint32_t)data[startIndex + 1] << 8)  |
           data[startIndex];
}

// Helper function to decode a float from the CAN packet payload (little-endian)
static float DecodeFloatFromDataLSBFirst(const uint8_t *data, int startIndex)
{
    union {
        uint32_t u;
        float f;
    } uint_to_float;
    uint_to_float.u = DecodeIntFromDataLSBFirst(data, startIndex);
    return uint_to_float.f;
}

// Helper function to construct CAN ID for CANSimple protocol
static uint16_t ConstructODriveCANID(uint8_t node_id, uint8_t cmd_id)
{
    return ((node_id & 0x1F) << 6) | (cmd_id & 0x3F);
}

// Helper function to validate node_id
static int ValidateNodeId(uint8_t node_id)
{
    if (node_id != NODE_ID_1 && node_id != NODE_ID_2) {
        return ERROR_INVALID_NODE_ID;
    }
    return ERROR_NONE;
}

// Helper function to get cmd_id from CAN ID
static uint8_t GetCmdIdFromCANID(uint16_t can_id)
{
    return can_id & 0x3F; // Lower 6 bits
}

// ODrive CANSimple Packet Assembly Functions
// Each function assembles a packet using the CANSimple protocol, with node_id and cmd_id in the CAN ID,
// and payload data in little-endian format.

// CMD ID: 0x000 - Get_Version (ODrive → Host)
// Signals: Protocol_Version (uint32_t), Hw_Version_Major (uint8_t), Hw_Version_Minor (uint8_t),
//          Hw_Version_Variant (uint8_t), Fw_Version_Major (uint8_t), Fw_Version_Minor (uint8_t),
//          Fw_Version_Revision (uint8_t), Fw_Version_Unreleased (uint8_t)
// DLC: 8 bytes (fits all signals)
void AssembleODriveGetVersionPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t protocol_version, uint8_t hw_version_major, uint8_t hw_version_minor,
    uint8_t hw_version_variant, uint8_t fw_version_major, uint8_t fw_version_minor,
    uint8_t fw_version_revision, uint8_t fw_version_unreleased)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_VERSION);
    packetToAssemble->dlc = 8; // 4 + 1 + 1 + 1 + 1
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, protocol_version, nextByte);
    nextByte += 4;
    packetToAssemble->data[nextByte++] = hw_version_major;
    packetToAssemble->data[nextByte++] = hw_version_minor;
    packetToAssemble->data[nextByte++] = hw_version_variant;
    packetToAssemble->data[nextByte] = fw_version_major;
}

// CMD ID: 0x001 - Heartbeat (ODrive → Host)
// Signals: Axis_Error (uint32_t), Axis_State (uint32_t), Procedure_Result (uint32_t), Trajectory_Done_Flag (uint8_t)
// Note: Omit Procedure_Result and Trajectory_Done_Flag to fit 8-byte limit
void AssembleODriveHeartbeatPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t axis_error, uint32_t axis_state, uint32_t procedure_result,
    uint8_t trajectory_done_flag)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_HEARTBEAT);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, axis_error, nextByte);
    nextByte += 4;
    PackIntIntoDataLSBFirst(packetToAssemble->data, axis_state, nextByte);
}

// CMD ID: 0x002 - Estop (Host → ODrive)
// Signals: None
void AssembleODriveEstopPacket(CANPacket *packetToAssemble, uint8_t node_id)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_ESTOP);
    packetToAssemble->dlc = 0; // No payload
    packetToAssemble->rtr = 0;
}

// CMD ID: 0x003 - Get_Error (ODrive → Host)
// Signals: Active_Errors (uint32_t), Disarm_Reason (uint32_t)
void AssembleODriveGetErrorPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t active_errors, uint32_t disarm_reason)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_ERROR);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, active_errors, nextByte);
    nextByte += 4;
    PackIntIntoDataLSBFirst(packetToAssemble->data, disarm_reason, nextByte);
}

// CMD ID: 0x004 - RxSdo (Host → ODrive)
// Signals: Opcode (uint8_t), Endpoint_ID (uint32_t), Reserved (uint32_t), Value (uint32_t)
// Note: Omit Value to fit 8-byte limit, set Reserved to 0
void AssembleODriveRxSdoPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint8_t opcode, uint32_t endpoint_id, uint32_t reserved, uint32_t value)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_RX_SDO);
    packetToAssemble->dlc = 5; // 1 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    packetToAssemble->data[nextByte++] = opcode;
    PackIntIntoDataLSBFirst(packetToAssemble->data, endpoint_id, nextByte);
}

// CMD ID: 0x005 - TxSdo (ODrive → Host)
// Signals: Reserved0 (uint32_t), Endpoint_ID (uint32_t), Reserved1 (uint32_t), Value (uint32_t)
// Note: Pack Endpoint_ID and Value, set Reserved to 0
void AssembleODriveTxSdoPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t reserved0, uint32_t endpoint_id, uint32_t reserved1, uint32_t value)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_TX_SDO);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, endpoint_id, nextByte);
    nextByte += 4;
    PackIntIntoDataLSBFirst(packetToAssemble->data, value, nextByte);
}

// CMD ID: 0x006 - Address (Host → ODrive, ODrive → Host)
// Signals: Node_ID (uint32_t), Serial_Number (uint32_t), Connection_ID (uint32_t)
// Note: Omit Connection_ID
void AssembleODriveAddressPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t node_id_value, uint32_t serial_number, uint32_t connection_id)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_ADDRESS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, node_id_value, nextByte);
    nextByte += 4;
    PackIntIntoDataLSBFirst(packetToAssemble->data, serial_number, nextByte);
}

// CMD ID: 0x007 - Set_Axis_State (Host → ODrive)
// Signals: Axis_Requested_State (uint32_t)
void AssembleODriveSetAxisStatePacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t axis_requested_state)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_AXIS_STATE);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, axis_requested_state, nextByte);
}

// CMD ID: 0x009 - Get_Encoder_Estimates (ODrive → Host)
// Signals: Pos_Estimate (float), Vel_Estimate (float)
void AssembleODriveGetEncoderEstimatesPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float pos_estimate, float vel_estimate)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_ENCODER_ESTIMATES);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, pos_estimate, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, vel_estimate, nextByte);
}

// CMD ID: 0x00B - Set_Controller_Mode (Host → ODrive)
// Signals: Control_Mode (uint32_t), Input_Mode (uint32_t)
void AssembleODriveSetControllerModePacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t control_mode, uint32_t input_mode)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_CONTROLLER_MODE);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, control_mode, nextByte);
    nextByte += 4;
    PackIntIntoDataLSBFirst(packetToAssemble->data, input_mode, nextByte);
}

// CMD ID: 0x00C - Set_Input_Pos (Host → ODrive)
// Signals: Input_Pos (float), Vel_FF (float), Torque_FF (float)
// Note: Omit Torque_FF
void AssembleODriveSetInputPosPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_pos, float vel_ff, float torque_ff)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_INPUT_POS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, input_pos, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, vel_ff, nextByte);
}

// CMD ID: 0x00D - Set_Input_Vel (Host → ODrive)
// Signals: Input_Vel (float), Input_Torque_FF (float)
void AssembleODriveSetInputVelPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_vel, float input_torque_ff)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_INPUT_VEL);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, input_vel, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, input_torque_ff, nextByte);
}

// CMD ID: 0x00E - Set_Input_Torque (Host → ODrive)
// Signals: Input_Torque (float)
void AssembleODriveSetInputTorquePacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_torque)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_INPUT_TORQUE);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, input_torque, nextByte);
}

// CMD ID: 0x00F - Set_Limits (Host → ODrive)
// Signals: Velocity_Limit (float), Current_Limit (float)
void AssembleODriveSetLimitsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float velocity_limit, float current_limit)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_LIMITS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, velocity_limit, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, current_limit, nextByte);
}

// CMD ID: 0x011 - Set_Traj_Vel_Limit (Host → ODrive)
// Signals: Traj_Vel_Limit (float)
void AssembleODriveSetTrajVelLimitPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_vel_limit)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_TRAJ_VEL_LIMIT);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, traj_vel_limit, nextByte);
}

// CMD ID: 0x012 - Set_Traj_Accel_Limits (Host → ODrive)
// Signals: Traj_Accel_Limit (float), Traj_Decel_Limit (float)
void AssembleODriveSetTrajAccelLimitsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_accel_limit, float traj_decel_limit)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_TRAJ_ACCEL_LIMITS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, traj_accel_limit, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, traj_decel_limit, nextByte);
}

// CMD ID: 0x013 - Set_Traj_Inertia (Host → ODrive)
// Signals: Traj_Inertia (float)
void AssembleODriveSetTrajInertiaPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_inertia)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_TRAJ_INERTIA);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, traj_inertia, nextByte);
}

// CMD ID: 0x014 - Get_Iq (ODrive → Host)
// Signals: Iq_Setpoint (float), Iq_Measured (float)
void AssembleODriveGetIqPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float iq_setpoint, float iq_measured)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_IQ);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, iq_setpoint, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, iq_measured, nextByte);
}

// CMD ID: 0x015 - Get_Temperature (ODrive → Host)
// Signals: FET_Temperature (float), Motor_Temperature (float)
void AssembleODriveGetTemperaturePacket(CANPacket *packetToAssemble, uint8_t node_id,
    float fet_temperature, float motor_temperature)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_TEMPERATURE);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, fet_temperature, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, motor_temperature, nextByte);
}

// CMD ID: 0x016 - Reboot (Host → ODrive)
// Signals: Action (uint32_t)
void AssembleODriveRebootPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t action)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_REBOOT);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, action, nextByte);
}

// CMD ID: 0x017 - Get_Bus_Voltage_Current (ODrive → Host)
// Signals: Bus_Voltage (float), Bus_Current (float)
void AssembleODriveGetBusVoltageCurrentPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float bus_voltage, float bus_current)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_BUS_VOLTAGE_CURRENT);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, bus_voltage, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, bus_current, nextByte);
}

// CMD ID: 0x018 - Clear_Errors (Host → ODrive)
// Signals: Identify (uint32_t)
void AssembleODriveClearErrorsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t identify)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_CLEAR_ERRORS);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackIntIntoDataLSBFirst(packetToAssemble->data, identify, nextByte);
}

// CMD ID: 0x019 - Set_Absolute_Position (Host → ODrive)
// Signals: Position (float)
void AssembleODriveSetAbsolutePositionPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float position)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_ABSOLUTE_POSITION);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, position, nextByte);
}

// CMD ID: 0x01A - Set_Pos_Gain (Host → ODrive)
// Signals: Pos_Gain (float)
void AssembleODriveSetPosGainPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float pos_gain)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_POS_GAIN);
    packetToAssemble->dlc = 4; // 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, pos_gain, nextByte);
}

// CMD ID: 0x01B - Set_Vel_Gains (Host → ODrive)
// Signals: Vel_Gain (float), Vel_Integrator_Gain (float)
void AssembleODriveSetVelGainsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float vel_gain, float vel_integrator_gain)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_SET_VEL_GAINS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, vel_gain, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, vel_integrator_gain, nextByte);
}

// CMD ID: 0x01C - Get_Torques (ODrive → Host)
// Signals: Torque_Target (float), Torque_Estimate (float)
void AssembleODriveGetTorquesPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float torque_target, float torque_estimate)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_TORQUES);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, torque_target, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, torque_estimate, nextByte);
}

// CMD ID: 0x01D - Get_Powers (ODrive → Host)
// Signals: Electrical_Power (float), Mechanical_Power (float)
void AssembleODriveGetPowersPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float electrical_power, float mechanical_power)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_GET_POWERS);
    packetToAssemble->dlc = 8; // 4 + 4
    packetToAssemble->rtr = 0;
    int nextByte = 0;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, electrical_power, nextByte);
    nextByte += 4;
    PackFloatIntoDataLSBFirst(packetToAssemble->data, mechanical_power, nextByte);
}

// CMD ID: 0x01F - Enter_DFU_Mode (Host → ODrive)
// Signals: None
void AssembleODriveEnterDFUModePacket(CANPacket *packetToAssemble, uint8_t node_id)
{
    if (ValidateNodeId(node_id) != ERROR_NONE) return;
    packetToAssemble->id = ConstructODriveCANID(node_id, ID_ODRIVE_ENTER_DFU_MODE);
    packetToAssemble->dlc = 0; // No payload
    packetToAssemble->rtr = 0;
}

// ODrive CANSimple Packet Decoding Functions
// Each function decodes a received packet, extracting signals in little-endian format.

// CMD ID: 0x000 - Get_Version (ODrive → Host)
void GetODriveGetVersionFromPacket(const CANPacket *packet,
    uint32_t *protocol_version, uint8_t *hw_version_major, uint8_t *hw_version_minor,
    uint8_t *hw_version_variant, uint8_t *fw_version_major, uint8_t *fw_version_minor,
    uint8_t *fw_version_revision, uint8_t *fw_version_unreleased)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_VERSION) return;
    int nextByte = 0;
    *protocol_version = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *hw_version_major = packet->data[nextByte++];
    *hw_version_minor = packet->data[nextByte++];
    *hw_version_variant = packet->data[nextByte++];
    *fw_version_major = packet->data[nextByte];
    // Note: Only 8 bytes available, so remaining signals are not decoded
    if (fw_version_minor) *fw_version_minor = 0;
    if (fw_version_revision) *fw_version_revision = 0;
    if (fw_version_unreleased) *fw_version_unreleased = 0;
}

// CMD ID: 0x001 - Heartbeat (ODrive → Host)
void GetODriveHeartbeatFromPacket(const CANPacket *packet,
    uint32_t *axis_error, uint32_t *axis_state, uint32_t *procedure_result,
    uint8_t *trajectory_done_flag)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_HEARTBEAT) return;
    int nextByte = 0;
    *axis_error = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *axis_state = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    // Note: Procedure_Result and Trajectory_Done_Flag omitted due to 8-byte limit
    if (procedure_result) *procedure_result = 0;
    if (trajectory_done_flag) *trajectory_done_flag = 0;
}

// CMD ID: 0x003 - Get_Error (ODrive → Host)
void GetODriveGetErrorFromPacket(const CANPacket *packet,
    uint32_t *active_errors, uint32_t *disarm_reason)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_ERROR) return;
    int nextByte = 0;
    *active_errors = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *disarm_reason = DecodeIntFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x005 - TxSdo (ODrive → Host)
void GetODriveTxSdoFromPacket(const CANPacket *packet,
    uint32_t *reserved0, uint32_t *endpoint_id, uint32_t *reserved1, uint32_t *value)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_TX_SDO) return;
    int nextByte = 0;
    *endpoint_id = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *value = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    // Note: Reserved fields are ignored as per CANSimple spec
    if (reserved0) *reserved0 = 0;
    if (reserved1) *reserved1 = 0;
}

// CMD ID: 0x006 - Address (Host → ODrive, ODrive → Host)
void GetODriveAddressFromPacket(const CANPacket *packet,
    uint32_t *node_id_value, uint32_t *serial_number, uint32_t *connection_id)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_ADDRESS) return;
    int nextByte = 0;
    *node_id_value = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *serial_number = DecodeIntFromDataLSBFirst(packet->data, nextByte);
    // Note: Connection_ID omitted due to 8-byte limit
    if (connection_id) *connection_id = 0;
}

// CMD ID: 0x009 - Get_Encoder_Estimates (ODrive → Host)
void GetODriveGetEncoderEstimatesFromPacket(const CANPacket *packet,
    float *pos_estimate, float *vel_estimate)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_ENCODER_ESTIMATES) return;
    int nextByte = 0;
    *pos_estimate = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *vel_estimate = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x014 - Get_Iq (ODrive → Host)
void GetODriveGetIqFromPacket(const CANPacket *packet,
    float *iq_setpoint, float *iq_measured)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_IQ) return;
    int nextByte = 0;
    *iq_setpoint = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *iq_measured = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x015 - Get_Temperature (ODrive → Host)
void GetODriveGetTemperatureFromPacket(const CANPacket *packet,
    float *fet_temperature, float *motor_temperature)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_TEMPERATURE) return;
    int nextByte = 0;
    *fet_temperature = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *motor_temperature = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x017 - Get_Bus_Voltage_Current (ODrive → Host)
void GetODriveGetBusVoltageCurrentFromPacket(const CANPacket *packet,
    float *bus_voltage, float *bus_current)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_BUS_VOLTAGE_CURRENT) return;
    int nextByte = 0;
    *bus_voltage = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *bus_current = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x01C - Get_Torques (ODrive → Host)
void GetODriveGetTorquesFromPacket(const CANPacket *packet,
    float *torque_target, float *torque_estimate)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_TORQUES) return;
    int nextByte = 0;
    *torque_target = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *torque_estimate = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// CMD ID: 0x01D - Get_Powers (ODrive → Host)
void GetODriveGetPowersFromPacket(const CANPacket *packet,
    float *electrical_power, float *mechanical_power)
{
    if (GetCmdIdFromCANID(packet->id) != ID_ODRIVE_GET_POWERS) return;
    int nextByte = 0;
    *electrical_power = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
    nextByte += 4;
    *mechanical_power = DecodeFloatFromDataLSBFirst(packet->data, nextByte);
}

// Function to send RTR request for ODrive "Get_" messages
int SendODriveRTRRequest(uint8_t node_id, uint8_t cmd_id)
{
    int result = ValidateNodeId(node_id);
    if (result != ERROR_NONE) return result;

    CANPacket packet;
    packet.id = ConstructODriveCANID(node_id, cmd_id);
    packet.dlc = 0; // No payload for RTR request
    packet.rtr = 1; // Set RTR flag
    return SendCANPacket(&packet);
}
