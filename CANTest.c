#include <stdbool.h>
#include <stdio.h>
#include "CANPacket.h"
#include "CANTest.h"
#include "Port.h"
#include "CANCommon.h"

//prints out test outcome
void testOutcome(char* testName, bool testResult){
    if(testResult){
        printf("%s: Passed\n", testName);
    }else{
        printf("%s: FAILED\n", testName);
    }

}

//tests functions in CANPacket.c
void CANPacketTest(){
    
    //Testing CANID construction
    uint16_t testID = ConstructCANID(1, 0xF, 0x3F);
    testOutcome("CAN ID Test", (testID == 0x7FF));

    //Testing CAN packet construction
    uint8_t testData[8];
    WriteSenderSerialAndPacketID(testData, 0xFF);

    CANPacket testPkt = ConstructCANPacket(testID, 8 ,testData);

    //Validating CAN packet

    //Validating Packet Priority
    testOutcome("Packet Priority Test", (GetPacketPriority(&testPkt) == 1));

    //Validating Packet ID
    testOutcome("Packet ID Test", (GetPacketID(&testPkt) == 0xFF));

    //Validating Receiver Device Serial Number
    testOutcome("Packet Receiver Device SN Test", (GetDeviceSerialNumber(&testPkt) == 0x3F));

    //Validating Sender Device Serial Number
    testOutcome("Packet Sender Device SN Test", (GetSenderDeviceSerialNumber(&testPkt) == getLocalDeviceSerial()));

    //Validating Receiver Device Group Code
    testOutcome("Packet Receiver Device GC Test", (GetDeviceGroupCode(&testPkt) == 0xF));

    //Validating Sender Device Group Code
    testOutcome("Packet Sender Device GC Test Successful", (GetSenderDeviceGroupCode(&testPkt) == getLocalDeviceGroup()));

    //Checking Functions that validate expected type
    testOutcome("Packet is in group", (PacketIsInGroup(&testPkt, 0xF)));
    testOutcome("Sender Packet is in group", (SenderPacketIsInGroup(&testPkt, getLocalDeviceGroup())));
    testOutcome("Sender Packet is in group", (SenderPacketIsOfDevice(&testPkt, getLocalDeviceSerial())));
    testOutcome("Packet is of ID", (PacketIsOfID(&testPkt, 0xFF)));

    testOutcome("Targets Device", (TargetsDevice(&testPkt, 0xF, 0x3F)));

    //Checking Functions for packing/unpacking

    //TODO ADD FOR LOOPS HERE TO CHECK FOR EVERY VALUE
    int32_t testDataToPack = 0xFFFFFFFF;
    uint8_t testDataToPackPtr;

    PackIntIntoDataMSBFirst(&testDataToPackPtr, testDataToPack, 0);
    int32_t unpackedData = DecodeBytesToIntMSBFirst(&testDataToPackPtr, 0, 3);

    testOutcome("Int Packing & Decoding", (testDataToPack == unpackedData));


    int16_t testShortDataToPack = 0xFFFF;
    uint8_t testShortDataToPackPtr;

    PackShortIntoDataMSBFirst(&testShortDataToPackPtr, testShortDataToPack, 0);
    int16_t unpackedShortData = DecodeBytesToIntMSBFirst(&testShortDataToPackPtr, 0, 1);

    testOutcome("Short Packing & Decoding", (testShortDataToPack == unpackedShortData));

}

void CANCommonTest(){
    CANPacket testPkt;

    //THIS FAILED FIND OUT WHY
    AssembleEmergencyStopPacket(&testPkt, 0xFF, 0xFF, 0xFF);
    testOutcome("Emergency Stop Packet Assembly & Get Emergency Stop Error Code", (0xFF == GetEmergencyStopErrorCode(&testPkt)));
}

int main(){
    CANPacketTest();
    CANCommonTest();

    return 0;
}