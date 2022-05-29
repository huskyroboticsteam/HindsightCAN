#ifndef CAN_SCIENCE_H
#define CAN_SCIENCE_H

#define CAN_SCIENCE_SENSOR_TEMPERATURE PACKET_TELEMETRY_SENSOR1
#define CAN_SCIENCE_SENSOR_UV PACKET_TELEMETRY_SENSOR2
#define CAN_SCIENCE_SENSOR_MOISTURE PACKET_TELEMETRY_SENSOR3

#define ID_SCIENCE_LAZY_SUSAN_POS_SET   ((uint8_t) 0x0C)
#define ID_SCIENCE_SERVO_SET            ((uint8_t) 0x0D)
#define ID_SCIENCE_CONT_SERVO_POWER_SET ((uint8_t) 0x0E)

#include "CANPacket.h"

void AssembleScienceServoPacket(CANPacket *packetToAssemble,
                                uint8_t targetGroup, uint8_t targetSerial,
                                uint8_t servo, uint8_t degrees);

void AssembleScienceLazySusanPosSetPacket(CANPacket *packetToAssemble,
                                          uint8_t targetDeviceGroup,
                                          uint8_t targetDeviceSerial,
                                          uint8_t position);

void AssembleScienceContServoPowerSetPacket(CANPacket *packetToAssemble,
											uint8_t targetDeviceGroup,
											uint8_t targetDeviceSerial,
											uint8_t servo,
											int8_t power);

uint8_t GetScienceServoIDFromPacket(CANPacket *packet);

uint8_t GetScienceServoAngleFromPacket(CANPacket *packet);

uint8_t GetScienceLazySusanPosFromPacket(CANPacket *packet);

int8_t GetScienceContServoPowerFromPacket(CANPacket *packet);

#endif
