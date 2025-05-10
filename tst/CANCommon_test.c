#include "./unity/unity.h"

#include "../CANCommon.h"

void setUp() {}    // Runs before each test
void tearDown() {} // Runs after each test

void assembleEmergencyStopPacket_withAddressZero_createsCorrectPacket()
{
    CANPacket *testPacket = {0};

    AssembleEmergencyStopPacket(testPacket, 0, 0, 1);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(assembleEmergencyStopPacket_withAddressZero_createsCorrectPacket);

    return UNITY_END();
}