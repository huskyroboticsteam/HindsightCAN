/* File:         Port.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin
 * Organization: Husky Robotics Team
 * 
 * This file includes function prototypes for all functions which must be
 * implemented for each ported device.  Just make a .c file called
 * Port[DeviceName].c and include this file in it.
 */
#pragma once

#include "CANPacket.h"

int SendCANPacket(CANPacket packet);
uint8_t getLocalDeviceSerial();
uint8_t getLocalDeviceGroup();

uint8_t getChipType();

//Chip type constants
//TODO: Find specific chip names
#define CHIP_TYPE_STM32Fxxx     0x01
#define CHIP_TYPE_PSOC4200Mxx   0x02
#define CHIP_TYPE_ATMEL8BITxx   0x03
