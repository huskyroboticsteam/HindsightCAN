#ifndef CAN_SCIENCE_H
#define CAN_SCIENCE_H

#define CAN_SCIENCE_SENSOR_GAS PACKET_TELEMETRY_SENSOR1
#define CAN_SCIENCE_SENSOR_UV PACKET_TELEMETRY_SENSOR2
#define CAN_SCIENCE_SENSOR_AIR_QUALITY PACKET_TELEMETRY_SENSOR3

void AssembleScienceSensorPullPacket(CANPacket *packetToAssemble,
uint8_t targetGroup,
uint8_t targetSerial,
uint8_t sensorCode);

void AssembleScienceMotorControlPacket(CANPacket *packetToAssemble,
uint8_t targetGroup,
uint8_t targetSerial,
uint8_t motor,
int16_t PWMSet);

void AssembleScienceServoPacket(CANPacket *packetToAssemble,
uint8_t targetGroup,
uint8_t targetSerial,
uint8_t servo,
uint8_t degrees);

int16_t GetScienceMotorPWMFromPacket(CANPacket *packet);

uint8_t GetScienceMotorIDFromPacket(CANPacket *packet);

uint8_t GetScienceServoIDFromPacket(CANPacket *packet);

uint8_t GetScienceServoAngleFromPacket(CANPacket *packet);

#endif