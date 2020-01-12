/* File:         CANSerialNumbers.h
 * Authors:      Dylan Tomberlin
 * Organization: Husky Robotics Team
 * 
 * This file defines the serial numbers for each device
 * on the Hindsight (PY2020 Rover). Sorted by Device Group
 * 
 * Serial Numbers are 6bits wide.
 */

// BROADCAST GROUP
// Use this serial number and the BROADCAST device group to
// broadcast a packet to all devices.
// Use this serial number and a specific device group to 
// broadcast a packet to all devices within a group.
#define DEVICE_SERIAL_BROADCAST         (uint8_t) 0x00

// JETSON GROUP
#define DEVICE_SERIAL_JETSON            (uint8_t) 0x01

// MOTOR UNIT GROUP
#define DEVICE_SERIAL_MOTOR_BASE_ROT          (uint8_t) 0x01
#define DEVICE_SERIAL_MOTOR_SHOULDER_ROT      (uint8_t) 0x02
#define DEVICE_SERIAL_MOTOR_ELBOW_ROT         (uint8_t) 0x03
#define DEVICE_SERIAL_MOTOR_WRIST_ROT         (uint8_t) 0x04
#define DEVICE_SERIAL_MOTOR_DIFF_WRIST_ROT_1  (uint8_t) 0x05
#define DEVICE_SERIAL_MOTOR_DIFF_WRIST_ROT_2  (uint8_t) 0x06
#define DEVICE_SERIAL_MOTOR_DIFF_HAND         (uint8_t) 0x07

#define DEVICE_SERIAL_MOTOR_CHASSIS_FL        (uint8_t) 0x08 //Front Left
#define DEVICE_SERIAL_MOTOR_CHASSIS_FR        (uint8_t) 0x09
#define DEVICE_SERIAL_MOTOR_CHASSIS_BL        (uint8_t) 0x0a 
#define DEVICE_SERIAL_MOTOR_CHASSIS_BR        (uint8_t) 0x0c //Back Right

        //if science has multiple motors, should name for function, not increment
#define DEVICE_SERIAL_MOTOR_BSCIENCE_1         (uint8_t) 0x0d

//Power Distro group
#define DEVICE_SERIAL_POWER_CHASSIS_MAIN        (uint8_t) 0x01
#define DEVICE_SERIAL_POWER_CHASSIS_DRIVE_1     (uint8_t) 0x02
#define DEVICE_SERIAL_POWER_CHASSIS_DRIVE_2     (uint8_t) 0x03
#define DEVICE_SERIAL_POWER_ARM_LOWER_1         (uint8_t) 0x04 //may have more arm
#define DEVICE_SERIAL_POWER_ARM_UPPER_1         (uint8_t) 0x05
#define DEVICE_SERIAL_POWER_SCIENCE             (uint8_t) 0x06

//Telemetry group
#define DEVICE_SERIAL_TELEM_LOCALIZATION        (uint8_t) 0x01
#define DEVICE_SERIAL_TELEM_IMU                 (uint8_t) 0x02
#define DEVICE_SERIAL_TELEM_TEMPERATURE         (uint8_t) 0x03
#define DEVICE_SERIAL_TELEM_SCIENCE             (uint8_t) 0x04

