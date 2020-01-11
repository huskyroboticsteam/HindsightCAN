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
#define DEVICE_SERIAL_BASE_ROT          (uint8_t) 0x01
#define DEVICE_SERIAL_SHOULDER_ROT      (uint8_t) 0x02
#define DEVICE_SERIAL_ELBOW_ROT         (uint8_t) 0x03
#define DEVICE_SERIAL_WRIST_ROT         (uint8_t) 0x04
#define DEVICE_SERIAL_DIFF_WRIST_ROT_1  (uint8_t) 0x05
#define DEVICE_SERIAL_DIFF_WRIST_ROT_2  (uint8_t) 0x06

