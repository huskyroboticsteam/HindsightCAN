/* File:         CANMotorUnit.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for CAN Packet manipulation
 * using the Hindsight CAN Communication standard. Specific files
 * for the motor unit boards.
 */



// Motor Unit Packet IDs
#define ID_MOTOR_UNIT_MODE_SEL          (uint8_t) 0x00
#define ID_MOTOR_UNIT_CHIP_TYPE_PULL    (uint8_t) 0x01
#define ID_MOTOR_UNIT_PWM_DIR_SET       (uint8_t) 0x02
#define ID_MOTOR_UNIT_PID_POS_TGT_SET   (uint8_t) 0x03
#define ID_MOTOR_UNIT_PID_P_SET         (uint8_t) 0x04
#define ID_MOTOR_UNIT_PID_I_SET         (uint8_t) 0x05
#define ID_MOTOR_UNIT_PID_D_SET         (uint8_t) 0x06
#define ID_MOTOR_UNIT_INIT              (uint8_t) 0x07
#define ID_MOTOR_UNIT_LIM_ALRT          (uint8_t) 0x08
#define ID_MOTOR_UNIT_ENC_PPJR_SET      (uint8_t) 0x09
#define ID_MOTOR_UNIT_MAX_JNT_REV_SET   (uint8_t) 0x0A