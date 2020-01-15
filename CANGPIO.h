/* File:         CANGPIO.h
 * Authors:      Jaden Bottemiller, Benton Kwong, Dylan Tomberlin.
 * Organization: Husky Robotics Team
 * 
 * This file includes fuction prototypes for the GPIO board CAN Communication
 * using the Hindsight CAN Communication standard.
 * Documentation: https://huskyroboticsteam.slite.com/app/channels/iU0BryG7M9/collections/aXvWTcIR6c/notes/4otlSFsSp2
 */

#pragma once

#include "CANPacket.h"

//Set PWM Frequency
void AssembleGPIOSetPWMFrequency(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t pwmChannel,
    uint16_t frequency);
uint8_t GetGPIOPWMChannelFromPacket(CANPacket *packet);
uint16_t GetGPIOPWMFrequencyFromPacket(CANPacket *packet);

//Set PWM Duty Cycle
void AssembleGPIOSetPWMDutyCycle(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t pwmChannel,
    uint16_t dutyCycle);
uint16_t GetGPIOPWMDutyCycle(CANPacket *packetToAssemble);


//Set ADC State
void AssembleGPIOSetADCState(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t ADCChannel,
    uint8_t state);
uint8_t GetGPIOADCChannelFromPacket(CANPacket *packet);
uint8_t GetGPIOADCStateFromPacket(CANPacket *packet);


//Set GPIO State
void AssembleGPIOSetPinState(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t GPIORegister,
    uint8_t bitNumber,
    uint8_t bitState);
uint8_t GetGPIORegisterFromPacket(CANPacket *packet);
uint8_t GetGPIOBitNumberFromPacket(CANPacket *packet);
uint8_t GetGPIOBitStateFromPacket(CANPacket *packet);


//GPIO Write 
void AssembleGPIOWrite(CANPacket *packetToAssemble, 
    uint8_t targetGroup, 
    uint8_t targetSerial,
    uint8_t GPIORegister,
    uint8_t bitNumber,
    uint8_t bitWriteValue);
uint8_t GetGPIOWriteValuesFromPacket(CANPacket *packet);