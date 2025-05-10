#include "./unity/unity.h"

#include "../CANCommon.h"

void setUp() {}    // Runs before each test
void tearDown() {} // Runs after each test

// Emergency Stop Packets
void assembleEmergencyStopPacket_withAddressZero_createsCorrectPacket()
{
    CANPacket *testPacket;

    AssembleEmergencyStopPacket(testPacket, 0, 0, 1);
}

// // so this function just runs assembleemergencystop but with groupcode and unitcode..?
// void AssembleGroupBroadcastingEmergencyStopPacket_createsCorrectPacket()
// {
//     CANPacket *testPacket;

//     AssembleGroupBroadcastingEmergencyStopPacket(testPacket, 0, GetEmergencyStopErrorCode(testPacket));
//     // GetDeviceGroupCode(testPacket) @ CANPacket ?
// }

// // and this one runs the GroupBroadcasting but I guess input for this one is running Group_Broadcast when 
// //  when GroupBroadcasting run by serial?
// void AssembleBroadcastEmergencyStopPacket_createsCorrectPacket()
// {
//     CANPacket *testPacket;

//     AssembleGroupBrodcastingEmergencyStopPacket(testPacket, 0, GetEmergencyStopErrorCode(testPacket));
//     // GetDeviceGroupCode(testPacket) @ CANPacket ?
// }

// // Heartbeat Packet

// // Inputs:
// //      packetToAssemble:       CAN Packet to assemble (will overwrite).
// //      broadcast:              1 if broadcast to all devices. 0 to return to MAIN_CPU / Jetson.
// //      heartbeatLeniencyCode:  Max time between heartbeats before system automatically enters a safe operating condition.
// //      timestamp:              Current timestamp as seen by the sender device. (ms)

// // packetToAssemble is it a different packet from other EmergencyStop packets input of canPacket or are they the same cus they going in testerrrrrrr hmm
// void AssembleHeartbeatPacket_toEverydevices()
// {
//     CANPacket *testPacket;

//     AssembleHeartbeatPacket(testPacket, 1, GetHeartbeatLeniencyCode(testPacket), GetHeartbeatTimestamp(testPacket));
// }

// void AssembleHeartbeatPacket_toMainCPU()
// {
//     CANPacket *testPacket;

//     AssembleHeartbeatPacket(testPacket, 0, GetHeartbeatLeniencyCode(testPacket), GetHeartbeatTimestamp(testPacket));
// }

// // also what is GetTimeBetweenHeartbeatPacket() for hmmmmmmmmm oh wait hold on
// // I do not know where PacketIsOfID() is but 'if (PacketIsOfID(packet, ID_HEARTBEAT))' hmm
// // FOUND IT its in CANPacket.c 
// // int GetPacketID(CANPacket *packet)
// // {
// //     return packet->data[0];
// // }

// // int PacketIsOfID(CANPacket *packet, uint8_t expectedID)
// // {
// //     return GetPacketID(packet) == expectedID;
// // }
// // so its checking ID_Heartbeat is THE ID ig?
// // okay so it returns heartbeat-heartbeat so i guess this is oh god cse 123 was too long ago like master function or sth?

// // lastHeartbeat
// // okay but what is the input for lastHeartbeat huh
// void AssembleHeartbeatPacket_toEverydevices()
// {
//     CANPacket *testPacket;

//     AssembleHeartbeatPacket(testPacket, 1, GetHeartbeatLeniencyCode(testPacket), GetTimeBetweenHeartbeatPacket(testPacket, ID_HEARTBEAT));
// }

// void AssembleHeartbeatPacket_toMainCPU()
// {
//     CANPacket *testPacket;

//     AssembleHeartbeatPacket(testPacket, 0, GetHeartbeatLeniencyCode(testPacket), GetTimeBetweenHeartbeatPacket(testPacket, ID_HEARTBEAT));
// }

// // Fail/Override Packet
// // again why is the parameter different hmmmmmmmmmmmmmmmmmmmmmmm
// // okay I don't think I am doing this right but should i keep going or call it a day 
// void AssembleFailReportPacket_IneedEunBeplease();

// // void AssembleOverrideProtectionPacket_


int main()
{
    UNITY_BEGIN();

    RUN_TEST(assembleEmergencyStopPacket_withAddressZero_createsCorrectPacket);
    // RUN_TEST(AssembleGroupBroadcastingEmergencyStopPacket_withAddressZero_createsCorrectPacket);

    return UNITY_END();
}

// . Emergency Stop Packets
//AssembleEmergencyStopPacket

//AssembleGroupBroadcastingEmergencyStopPacket

//AssembleBrodcastEmergencyStopPacket

//GetEmergencyStopErrorCode
