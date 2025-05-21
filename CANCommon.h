/* File:         CANCommon.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for Common Mode CAN Communication
 * using the Hindsight CAN Communication standard.
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 * 
 * Modified to include CANSimple protocol command IDs, packet assembly, decoding functions, and RTR support for ODrive BLDC motor driver.
 */

#pragma once

#include "CANPacket.h"

void AssembleEmergencyStopPacket(CANPacket *packet,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerialNumber,
    uint8_t errorCode);
void AssembleGroupBroadcastingEmergencyStopPacket(CANPacket *packet, 
    uint8_t groupCode, 
    uint8_t errorCode);
void AssembleBrodcastEmergencyStopPacket(CANPacket *packet,
    uint8_t errorCode);
uint8_t GetEmergencyStopErrorCode(CANPacket *packet);

uint32_t GetTimeBetweenHeartbeatPacket(CANPacket *packet, uint32_t lastHeartbeat);
uint32_t GetHeartbeatTimeStamp(CANPacket *packet);
void AssembleHeartbeatPacket(CANPacket *packetToAssemble, 
    int broadcast, 
    uint8_t heartbeatLeniencyCode,
    uint32_t timestamp);

void AssembleFailReportPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t failedPacketID);

void AssembleOverrideProtectionPacket(CANPacket *packetToAssemble, uint8_t targetGroup, uint8_t targetSerial);

//Chip type pull
void AssembleChipTypePullPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial);
void AssembleChipTypeReportPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial);
uint8_t GetChipTypeFromPacket(CANPacket *packet);

void AssembleTelemetryTimingPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial, 
    uint8_t telemetryTypeCode,
    uint32_t msBetweenReports);
uint32_t GetTelemetryTimingFromPacket(CANPacket *packetToAssemble);

void AssembleTelemetryPullPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial, 
    uint8_t telemetryTypeCode);

void AssembleTelemetryReportPacket(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t telemetryTypeCode,
    int32_t data);

int32_t DecodeTelemetryDataSigned(CANPacket *packet);
uint32_t DecodeTelemetryDataUnsigned(CANPacket *packet);
uint8_t DecodeTelemetryType(CANPacket *packet);

void AssembleRGBColorPacket(CANPacket *packetToAssemble,
    uint8_t targetGroup,
    uint8_t targetSerial,
    uint8_t addrLED,
    uint8_t R,
    uint8_t G,
    uint8_t B);

// Common Mode Packet IDs
#define ID_ESTOP                        (uint8_t) 0xF0
#define ID_HEARTBEAT                    (uint8_t) 0xF1
#define ID_FAIL_REPORT                  (uint8_t) 0xF2
#define ID_OVRD_PROTECTION              (uint8_t) 0xF3
#define ID_TELEMETRY_TIMING             (uint8_t) 0xF4
#define ID_TELEMETRY_PULL               (uint8_t) 0xF5
#define ID_TELEMETRY_REPORT             (uint8_t) 0xF6
#define ID_LED_COLOR                    (uint8_t) 0xF7
#define ID_CHIP_TYPE_PULL               (uint8_t) 0xF8
#define ID_CHIP_TYPE_REP                (uint8_t) 0xF9

// DLC Common Mode Packets 
#define DLC_ESTOP                        (uint8_t) 0x04
#define DLC_HEARTBEAT                    (uint8_t) 0x08
#define DLC_FAIL_REPORT                  (uint8_t) 0x04
#define DLC_OVRD_PROTECTION              (uint8_t) 0x01
#define DLC_TELEMETRY_TIMING             (uint8_t) 0x08
#define DLC_TELEMETRY_PULL               (uint8_t) 0x04
#define DLC_TELEMETRY_REPORT             (uint8_t) 0x08
#define DLC_LED_COLOR                    (uint8_t) 0x06
#define DLC_CHIP_TYPE_PULL               (uint8_t) 0x04
#define DLC_CHIP_TYPE_REP                (uint8_t) 0x04

//Packet priorities
#define PRIO_CHIP_TYPE_REP               PACKET_PRIORITY_NORMAL

// Telemetry Types
#define PACKET_TELEMETRY_VOLTAGE        ((uint8_t) 0x00)
#define PACKET_TELEMETRY_CURRENT        ((uint8_t) 0x01)
#define PACKET_TELEMETRY_PWR_RAIL_STATE ((uint8_t) 0x02)
#define PACKET_TELEMETRY_TEMPERATURE    ((uint8_t) 0x03)
#define PACKET_TELEMETRY_ANG_POSITION   ((uint8_t) 0x04)
#define PACKET_TELEMETRY_GPS_LAT        ((uint8_t) 0x05)
#define PACKET_TELEMETRY_GPS_LON        ((uint8_t) 0x06)
#define PACKET_TELEMETRY_MAG_DIR        ((uint8_t) 0x07)
#define PACKET_TELEMETRY_ACCEL_X        ((uint8_t) 0x08)
#define PACKET_TELEMETRY_ACCEL_Y        ((uint8_t) 0x09)
#define PACKET_TELEMETRY_ACCEL_Z        ((uint8_t) 0x0A)
#define PACKET_TELEMETRY_GYRO_X         ((uint8_t) 0x0B)
#define PACKET_TELEMETRY_GYRO_Y         ((uint8_t) 0x0C)
#define PACKET_TELEMETRY_GYRO_Z         ((uint8_t) 0x0D)
#define PACKET_TELEMETRY_LIM_SW_STATE   ((uint8_t) 0x0E)
#define PACKET_TELEMETRY_ADC_RAW        ((uint8_t) 0x0F)
#define PACKET_TELEMETRY_GPIO_STATE     ((uint8_t) 0x10)
#define PACKET_TELEMETRY_CHIP_TYPE      ((uint8_t) 0x11)
#define PACKET_TELEMETRY_QUATERNION_W   ((uint8_t) 0x12)
#define PACKET_TELEMETRY_QUATERNION_X   ((uint8_t) 0x13)
#define PACKET_TELEMETRY_QUATERNION_Y   ((uint8_t) 0x14)
#define PACKET_TELEMETRY_QUATERNION_Z   ((uint8_t) 0x15)
#define PACKET_TELEMETRY_SENSOR1        ((uint8_t) 0x16)
#define PACKET_TELEMETRY_SENSOR2        ((uint8_t) 0x17)
#define PACKET_TELEMETRY_SENSOR3        ((uint8_t) 0x18)
#define PACKET_TELEMETRY_SENSOR4        ((uint8_t) 0x19)
#define PACKET_TELEMETRY_SENSOR5        ((uint8_t) 0x1A)
#define PACKET_TELEMETRY_SENSOR6        ((uint8_t) 0x1B)

// ESTOP ERROR CODES
#define ESTOP_ERR_GENERAL               (uint8_t) 0x00
// MORE TBD...

// Node IDs for the two ODrive boards
#define NODE_ID_1                       (uint8_t) 0x01
#define NODE_ID_2                       (uint8_t) 0x02

// CANSimple Protocol Command IDs for ODrive BLDC Motor Driver
// Format: 11-bit CAN ID = [node_id (5 bits) | cmd_id (6 bits)]
// Below are the cmd_id values and associated metadata (direction, signals).

// CMD ID: 0x000 - Get_Version (ODrive → Host)
// Signals: Protocol_Version (uint32_t), Hw_Version_Major (uint8_t), Hw_Version_Minor (uint8_t),
//          Hw_Version_Variant (uint8_t), Fw_Version_Major (uint8_t), Fw_Version_Minor (uint8_t),
//          Fw_Version_Revision (uint8_t), Fw_Version_Unreleased (uint8_t)
#define ID_ODRIVE_GET_VERSION           (uint8_t) 0x000

// CMD ID: 0x001 - Heartbeat (ODrive → Host)
// Signals: Axis_Error (uint32_t), Axis_State (uint32_t), Procedure_Result (uint32_t), Trajectory_Done_Flag (uint8_t)
#define ID_ODRIVE_HEARTBEAT             (uint8_t) 0x001

// CMD ID: 0x002 - Estop (Host → ODrive)
// Signals: None
#define ID_ODRIVE_ESTOP                 (uint8_t) 0x002

// CMD ID: 0x003 - Get_Error (ODrive → Host)
// Signals: Active_Errors (uint32_t), Disarm_Reason (uint32_t)
#define ID_ODRIVE_GET_ERROR             (uint8_t) 0x003

// CMD ID: 0x004 - RxSdo (Host → ODrive)
// Signals: Opcode (uint8_t), Endpoint_ID (uint32_t), Reserved (uint32_t), Value (uint32_t)
#define ID_ODRIVE_RX_SDO                (uint8_t) 0x004

// CMD ID: 0x005 - TxSdo (ODrive → Host)
// Signals: Reserved0 (uint32_t), Endpoint_ID (uint32_t), Reserved1 (uint32_t), Value (uint32_t)
#define ID_ODRIVE_TX_SDO                (uint8_t) 0x005

// CMD ID: 0x006 - Address (Host → ODrive, ODrive → Host)
// Signals: Node_ID (uint32_t), Serial_Number (uint32_t), Connection_ID (uint32_t)
#define ID_ODRIVE_ADDRESS               (uint8_t) 0x006

// CMD ID: 0x007 - Set_Axis_State (Host → ODrive)
// Signals: Axis_Requested_State (uint32_t)
#define ID_ODRIVE_SET_AXIS_STATE        (uint8_t) 0x007

// CMD ID: 0x009 - Get_Encoder_Estimates (ODrive → Host)
// Signals: Pos_Estimate (float), Vel_Estimate (float)
#define ID_ODRIVE_GET_ENCODER_ESTIMATES (uint8_t) 0x009

// CMD ID: 0x00B - Set_Controller_Mode (Host → ODrive)
// Signals: Control_Mode (uint32_t), Input_Mode (uint32_t)
#define ID_ODRIVE_SET_CONTROLLER_MODE   (uint8_t) 0x00B

// CMD ID: 0x00C - Set_Input_Pos (Host → ODrive)
// Signals: Input_Pos (float), Vel_FF (float), Torque_FF (float)
#define ID_ODRIVE_SET_INPUT_POS         (uint8_t) 0x00C

// CMD ID: 0x00D - Set_Input_Vel (Host → ODrive)
// Signals: Input_Vel (float), Input_Torque_FF (float)
#define ID_ODRIVE_SET_INPUT_VEL         (uint8_t) 0x00D

// CMD ID: 0x00E - Set_Input_Torque (Host → ODrive)
// Signals: Input_Torque (float)
#define ID_ODRIVE_SET_INPUT_TORQUE      (uint8_t) 0x00E

// CMD ID: 0x00F - Set_Limits (Host → ODrive)
// Signals: Velocity_Limit (float), Current_Limit (float)
#define ID_ODRIVE_SET_LIMITS            (uint8_t) 0x00F

// CMD ID: 0x011 - Set_Traj_Vel_Limit (Host → ODrive)
// Signals: Traj_Vel_Limit (float)
#define ID_ODRIVE_SET_TRAJ_VEL_LIMIT    (uint8_t) 0x011

// CMD ID: 0x012 - Set_Traj_Accel_Limits (Host → ODrive)
// Signals: Traj_Accel_Limit (float), Traj_Decel_Limit (float)
#define ID_ODRIVE_SET_TRAJ_ACCEL_LIMITS (uint8_t) 0x012

// CMD ID: 0x013 - Set_Traj_Inertia (Host → ODrive)
// Signals: Traj_Inertia (float)
#define ID_ODRIVE_SET_TRAJ_INERTIA      (uint8_t) 0x013

// CMD ID: 0x014 - Get_Iq (ODrive → Host)
// Signals: Iq_Setpoint (float), Iq_Measured (float)
#define ID_ODRIVE_GET_IQ                (uint8_t) 0x014

// CMD ID: 0x015 - Get_Temperature (ODrive → Host)
// Signals: FET_Temperature (float), Motor_Temperature (float)
#define ID_ODRIVE_GET_TEMPERATURE       (uint8_t) 0x015

// CMD ID: 0x016 - Reboot (Host → ODrive)
// Signals: Action (uint32_t)
#define ID_ODRIVE_REBOOT                (uint8_t) 0x016

// CMD ID: 0x017 - Get_Bus_Voltage_Current (ODrive → Host)
// Signals: Bus_Voltage (float), Bus_Current (float)
#define ID_ODRIVE_GET_BUS_VOLTAGE_CURRENT (uint8_t) 0x017

// CMD ID: 0x018 - Clear_Errors (Host → ODrive)
// Signals: Identify (uint32_t)
#define ID_ODRIVE_CLEAR_ERRORS          (uint8_t) 0x018

// CMD ID: 0x019 - Set_Absolute_Position (Host → ODrive)
// Signals: Position (float)
#define ID_ODRIVE_SET_ABSOLUTE_POSITION (uint8_t) 0x019

// CMD ID: 0x01A - Set_Pos_Gain (Host → ODrive)
// Signals: Pos_Gain (float)
#define ID_ODRIVE_SET_POS_GAIN          (uint8_t) 0x01A

// CMD ID: 0x01B - Set_Vel_Gains (Host → ODrive)
// Signals: Vel_Gain (float), Vel_Integrator_Gain (float)
#define ID_ODRIVE_SET_VEL_GAINS         (uint8_t) 0x01B

// CMD ID: 0x01C - Get_Torques (ODrive → Host)
// Signals: Torque_Target (float), Torque_Estimate (float)
#define ID_ODRIVE_GET_TORQUES           (uint8_t) 0x01C

// CMD ID: 0x01D - Get_Powers (ODrive → Host)
// Signals: Electrical_Power (float), Mechanical_Power (float)
#define ID_ODRIVE_GET_POWERS            (uint8_t) 0x01D

// CMD ID: 0x01F - Enter_DFU_Mode (Host → ODrive)
// Signals: None
#define ID_ODRIVE_ENTER_DFU_MODE        (uint8_t) 0x01F

// Function prototypes for assembling ODrive CANSimple packets
void AssembleODriveGetVersionPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t protocol_version, uint8_t hw_version_major, uint8_t hw_version_minor,
    uint8_t hw_version_variant, uint8_t fw_version_major, uint8_t fw_version_minor,
    uint8_t fw_version_revision, uint8_t fw_version_unreleased);
void AssembleODriveHeartbeatPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t axis_error, uint32_t axis_state, uint32_t procedure_result,
    uint8_t trajectory_done_flag);
void AssembleODriveEstopPacket(CANPacket *packetToAssemble, uint8_t node_id);
void AssembleODriveGetErrorPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t active_errors, uint32_t disarm_reason);
void AssembleODriveRxSdoPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint8_t opcode, uint32_t endpoint_id, uint32_t reserved, uint32_t value);
void AssembleODriveTxSdoPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t reserved0, uint32_t endpoint_id, uint32_t reserved1, uint32_t value);
void AssembleODriveAddressPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t node_id_value, uint32_t serial_number, uint32_t connection_id);
void AssembleODriveSetAxisStatePacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t axis_requested_state);
void AssembleODriveGetEncoderEstimatesPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float pos_estimate, float vel_estimate);
void AssembleODriveSetControllerModePacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t control_mode, uint32_t input_mode);
void AssembleODriveSetInputPosPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_pos, float vel_ff, float torque_ff);
void AssembleODriveSetInputVelPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_vel, float input_torque_ff);
void AssembleODriveSetInputTorquePacket(CANPacket *packetToAssemble, uint8_t node_id,
    float input_torque);
void AssembleODriveSetLimitsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float velocity_limit, float current_limit);
void AssembleODriveSetTrajVelLimitPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_vel_limit);
void AssembleODriveSetTrajAccelLimitsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_accel_limit, float traj_decel_limit);
void AssembleODriveSetTrajInertiaPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float traj_inertia);
void AssembleODriveGetIqPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float iq_setpoint, float iq_measured);
void AssembleODriveGetTemperaturePacket(CANPacket *packetToAssemble, uint8_t node_id,
    float fet_temperature, float motor_temperature);
void AssembleODriveRebootPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t action);
void AssembleODriveGetBusVoltageCurrentPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float bus_voltage, float bus_current);
void AssembleODriveClearErrorsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    uint32_t identify);
void AssembleODriveSetAbsolutePositionPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float position);
void AssembleODriveSetPosGainPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float pos_gain);
void AssembleODriveSetVelGainsPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float vel_gain, float vel_integrator_gain);
void AssembleODriveGetTorquesPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float torque_target, float torque_estimate);
void AssembleODriveGetPowersPacket(CANPacket *packetToAssemble, uint8_t node_id,
    float electrical_power, float mechanical_power);
void AssembleODriveEnterDFUModePacket(CANPacket *packetToAssemble, uint8_t node_id);

// Function prototypes for decoding ODrive CANSimple packets
void GetODriveGetVersionFromPacket(const CANPacket *packet,
    uint32_t *protocol_version, uint8_t *hw_version_major, uint8_t *hw_version_minor,
    uint8_t *hw_version_variant, uint8_t *fw_version_major, uint8_t *fw_version_minor,
    uint8_t *fw_version_revision, uint8_t *fw_version_unreleased);
void GetODriveHeartbeatFromPacket(const CANPacket *packet,
    uint32_t *axis_error, uint32_t *axis_state, uint32_t *procedure_result,
    uint8_t *trajectory_done_flag);
void GetODriveGetErrorFromPacket(const CANPacket *packet,
    uint32_t *active_errors, uint32_t *disarm_reason);
void GetODriveTxSdoFromPacket(const CANPacket *packet,
    uint32_t *reserved0, uint32_t *endpoint_id, uint32_t *reserved1, uint32_t *value);
void GetODriveAddressFromPacket(const CANPacket *packet,
    uint32_t *node_id_value, uint32_t *serial_number, uint32_t *connection_id);
void GetODriveGetEncoderEstimatesFromPacket(const CANPacket *packet,
    float *pos_estimate, float *vel_estimate);
void GetODriveGetIqFromPacket(const CANPacket *packet,
    float *iq_setpoint, float *iq_measured);
void GetODriveGetTemperatureFromPacket(const CANPacket *packet,
    float *fet_temperature, float *motor_temperature);
void GetODriveGetBusVoltageCurrentFromPacket(const CANPacket *packet,
    float *bus_voltage, float *bus_current);
void GetODriveGetTorquesFromPacket(const CANPacket *packet,
    float *torque_target, float *torque_estimate);
void GetODriveGetPowersFromPacket(const CANPacket *packet,
    float *electrical_power, float *mechanical_power);

// Function to send RTR request for ODrive "Get_" messages
int SendODriveRTRRequest(uint8_t node_id, uint8_t cmd_id);

#endif
