/* File:         Port.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin
 * Organization: Husky Robotics Team
 * 
 * This file includes function prototypes for all functions which must be
 * implemented for each ported device.  Just make a .c file called
 * Port[DeviceName].c based on PortTemplate.
 *
 * Compile insturctions: in addition to cross compiling, you will need to
 * define the constant CHIP_TYPE in your compiler options.  On GCC, use
 * -D CHIP_TYPE=CHIP_TYPE_TEMPLATE
 *  But obviously substitute template for your own constant.  Add it to the list
 *  of constants below if its not already been defined.
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 */
#pragma once

#include "CANPacket.h"

int SendCANPacket(CANPacket *packetToSend);
CANPacket ReceiveCANPacket();

uint8_t getLocalDeviceSerial();
uint8_t getLocalDeviceGroup();

uint8_t getChipType();

//Chip type constants
//TODO: Find specific chip names
#define CHIP_TYPE_TEMPLATE      0x00
#define CHIP_TYPE_STM32Fxxx     0x01
#define CHIP_TYPE_PSOC4200Mxx   0x02
#define CHIP_TYPE_ATMEL8BITxx   0x03
