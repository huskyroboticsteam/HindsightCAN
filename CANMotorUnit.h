/* File:         CANMotorUnit.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for CAN Packet manipulation
 * using the Hindsight CAN Communication standard. Specific files
 * for the motor unit boards.
 */

#include "CANPacket.h"

// TODO: Add parameters to packet assembly
void AssembleModeSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t mode);
uint8_t GetModeFromPacket(CANPacket *packet);

void AssembleChipTypePullPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t yourChipType);
void AssembleChipTypeReportPacket(CANPacket *packetToAssemble,
    uint8_t senderDeviceGroup,
    uint8_t senderDeviceSerial,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    uint8_t chipType);
uint8_t GetChipTypeFromPacket(CANPacket *packet);

void AssemblePWMDirSetPacket(CANPacket *packetToAssemble, 
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t PWMSet);
int32_t GetPWMFromPacket(CANPacket *packet);
int32_t GetDirectionFromPacket(CANPacket *packet);

void AssemblePIDTargetSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t target);
uint8_t GetPIDTargetFromPacket(CANPacket *packet);

void AssemblePSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t pCoef);
int32_t GetPFromPacket(CANPacket *packet);

void AssembleISetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t iCoef);
int32_t GetIFromPacket(CANPacket *packet);

void AssembleDSetPacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial,
    int32_t dCoef);
int32_t GetDFromPacket(CANPacket *packet);

void AssembleInitializePacket(CANPacket *packetToAssemble,
    uint8_t targetDeviceGroup,
    uint8_t targetDeviceSerial);

void AssembleLimitSwitchAlertPacket(CANPacket *packetToAssemble);

void AssembleEncoderPPJRSetPacket(CANPacket *packetToAssemble);
uint32_t GetEncoderPPJRFromPacket(CANPacket *packet);

void AssembleMaxJointRevolutionPacket(CANPacket *packetToAssemble);
uint32_t GetMaxJointRevolutionsFromPacket(CANPacket *packet);

// Motor Unit Packet IDs
#define ID_MOTOR_UNIT_MODE_SEL          (uint8_t) 0x00
#define ID_MOTOR_UNIT_CHIP_TYPE_PULL    (uint8_t) 0x01
#define ID_MOTOR_UNIT_CHIP_TYPE_REP     (uint8_t) 0x02
#define ID_MOTOR_UNIT_PWM_DIR_SET       (uint8_t) 0x03
#define ID_MOTOR_UNIT_PID_POS_TGT_SET   (uint8_t) 0x04
#define ID_MOTOR_UNIT_PID_P_SET         (uint8_t) 0x05
#define ID_MOTOR_UNIT_PID_I_SET         (uint8_t) 0x06
#define ID_MOTOR_UNIT_PID_D_SET         (uint8_t) 0x07
#define ID_MOTOR_UNIT_INIT              (uint8_t) 0x08
#define ID_MOTOR_UNIT_LIM_ALRT          (uint8_t) 0x09
#define ID_MOTOR_UNIT_ENC_PPJR_SET      (uint8_t) 0x0A
#define ID_MOTOR_UNIT_MAX_JNT_REV_SET   (uint8_t) 0x0B

// Motor Unit Mode IDs
#define MOTOR_UNIT_MODE_PWM             (uint8_t) 0x00
#define MOTOR_UNIT_MODE_PID             (uint8_t) 0x01