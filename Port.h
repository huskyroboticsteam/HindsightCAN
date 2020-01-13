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
