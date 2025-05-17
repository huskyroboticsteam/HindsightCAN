/* File:         CANCommon.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for Common Mode CAN Communication
 * using the Hindsight CAN Communication standard.
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 * 
 * Modified to include CANSimple protocol command IDs for ODrive BLDC motor driver.
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

// CANSimple Protocol Command IDs for ODrive BLDC Motor Driver
// Format: 11-bit CAN ID = [node_id (5 bits) | cmd_id (6 bits)]
// Below are the cmd_id values and associated metadata (direction, signals).
// Data alignment for signals will be handled later.

// CMD ID: 0x000 - Get_Version (ODrive → Host)
// Signals: Protocol_Version, Hw_Version_Major, Hw_Version_Minor, Hw_Version_Variant,
//          Fw_Version_Major, Fw_Version_Minor, Fw_Version_Revision, Fw_Version_Unreleased
#define ID_ODRIVE_GET_VERSION           (uint8_t) 0x000

// CMD ID: 0x001 - Heartbeat (ODrive → Host)
// Signals: Axis_Error, Axis_State, Procedure_Result, Trajectory_Done_Flag
#define ID_ODRIVE_HEARTBEAT             (uint8_t) 0x001

// CMD ID: 0x002 - Estop (Host → ODrive)
// Signals: None
#define ID_ODRIVE_ESTOP                 (uint8_t) 0x002

// CMD ID: 0x003 - Get_Error (ODrive → Host)
// Signals: Active_Errors, Disarm_Reason
#define ID_ODRIVE_GET_ERROR             (uint8_t) 0x003

// CMD ID: 0x004 - RxSdo (Host → ODrive)
// Signals: Opcode, Endpoint_ID, Reserved, Value
#define ID_ODRIVE_RX_SDO                (uint8_t) 0x004

// CMD ID: 0x005 - TxSdo (ODrive → Host)
// Signals: Reserved0, Endpoint_ID, Reserved1, Value
#define ID_ODRIVE_TX_SDO                (uint8_t) 0x005

// CMD ID: 0x006 - Address (Host → ODrive, ODrive → Host)
// Signals: Node_ID, Serial_Number, Connection_ID
#define ID_ODRIVE_ADDRESS               (uint8_t) 0x006

// CMD ID: 0x007 - Set_Axis_State (Host → ODrive)
// Signals: Axis_Requested_State
#define ID_ODRIVE_SET_AXIS_STATE        (uint8_t) 0x007

// CMD ID: 0x009 - Get_Encoder_Estimates (ODrive → Host)
// Signals: Pos_Estimate, Vel_Estimate
#define ID_ODRIVE_GET_ENCODER_ESTIMATES (uint8_t) 0x009

// CMD ID: 0x00B - Set_Controller_Mode (Host → ODrive)
// Signals: Control_Mode, Input_Mode
#define ID_ODRIVE_SET_CONTROLLER_MODE   (uint8_t) 0x00B

// CMD ID: 0x00C - Set_Input_Pos (Host → ODrive)
// Signals: Input_Pos, Vel_FF, Torque_FF
#define ID_ODRIVE_SET_INPUT_POS         (uint8_t) 0x00C

// CMD ID: 0x00D - Set_Input_Vel (Host → ODrive)
// Signals: Input_Vel, Input_Torque_FF
#define ID_ODRIVE_SET_INPUT_VEL         (uint8_t) 0x00D

// CMD ID: 0x00E - Set_Input_Torque (Host → ODrive)
// Signals: Input_Torque
#define ID_ODRIVE_SET_INPUT_TORQUE      (uint8_t) 0x00E

// CMD ID: 0x00F - Set_Limits (Host → ODrive)
// Signals: Velocity_Limit, Current_Limit
#define ID_ODRIVE_SET_LIMITS            (uint8_t) 0x00F

// CMD ID: 0x011 - Set_Traj_Vel_Limit (Host → ODrive)
// Signals: Traj_Vel_Limit
#define ID_ODRIVE_SET_TRAJ_VEL_LIMIT    (uint8_t) 0x011

// CMD ID: 0x012 - Set_Traj_Accel_Limits (Host → ODrive)
// Signals: Traj_Accel_Limit, Traj_Decel_Limit
#define ID_ODRIVE_SET_TRAJ_ACCEL_LIMITS (uint8_t) 0x012

// CMD ID: 0x013 - Set_Traj_Inertia (Host → ODrive)
// Signals: Traj_Inertia
#define ID_ODRIVE_SET_TRAJ_INERTIA      (uint8_t) 0x013

// CMD ID: 0x014 - Get_Iq (ODrive → Host)
// Signals: Iq_Setpoint, Iq_Measured
#define ID_ODRIVE_GET_IQ                (uint8_t) 0x014

// CMD ID: 0x015 - Get_Temperature (ODrive → Host)
// Signals: FET_Temperature, Motor_Temperature
#define ID_ODRIVE_GET_TEMPERATURE       (uint8_t) 0x015

// CMD ID: 0x016 - Reboot (Host → ODrive)
// Signals: Action
#define ID_ODRIVE_REBOOT                (uint8_t) 0x016

// CMD ID: 0x017 - Get_Bus_Voltage_Current (ODrive → Host)
// Signals: Bus_Voltage, Bus_Current
#define ID_ODRIVE_GET_BUS_VOLTAGE_CURRENT (uint8_t) 0x017

// CMD ID: 0x018 - Clear_Errors (Host → ODrive)
// Signals: Identify
#define ID_ODRIVE_CLEAR_ERRORS          (uint8_t) 0x018

// CMD ID: 0x019 - Set_Absolute_Position (Host → ODrive)
// Signals: Position
#define ID_ODRIVE_SET_ABSOLUTE_POSITION (uint8_t) 0x019

// CMD ID: 0x01A - Set_Pos_Gain (Host → ODrive)
// Signals: Pos_Gain
#define ID_ODRIVE_SET_POS_GAIN          (uint8_t) 0x01A

// CMD ID: 0x01B - Set_Vel_Gains (Host → ODrive)
// Signals: Vel_Gain, Vel_Integrator_Gain
#define ID_ODRIVE_SET_VEL_GAINS         (uint8_t) 0x01B

// CMD ID: 0x01C - Get_Torques (ODrive → Host)
// Signals: Torque_Target, Torque_Estimate
#define ID_ODRIVE_GET_TORQUES           (uint8_t) 0x01C

// CMD ID: 0x01D - Get_Powers (ODrive → Host)
// Signals: Electrical_Power, Mechanical_Power
#define ID_ODRIVE_GET_POWERS            (uint8_t) 0x01D

// CMD ID: 0x01F - Enter_DFU_Mode (Host → ODrive)
// Signals: None
#define ID_ODRIVE_ENTER_DFU_MODE        (uint8_t) 0x01F

#endif
