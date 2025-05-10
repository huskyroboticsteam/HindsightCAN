CC = gcc
C_FLAGS = -g -Wall -std=c17

BUILD = build
UNITY_SRC = unity.c unity.h

# Find all test source files (exclude unity.c)
TEST_SRC := $(filter-out unity.c, $(wildcard *.c))
TEST_BIN := $(patsubst %.c,$(BUILD)/%,$(TEST_SRC))

all: run_tests

run_tests: $(TEST_BIN)
	@echo "Running unit tests:"
	@for test in $(TEST_BIN); do echo "▶ $$test"; ./$$test || exit 1; done

common_tests: out unity.o Port.o CANPacket.o CANCommon.o CANCommon_test.c
	$(CC) $(C_FLAGS) -o out/CANCommon_test CANCommon_test.c build/unity.o build/Port.o build/CANPacket.o build/CANCommon.o

CANPacket.o: $(BUILD) ../CANPacket.c ../CANPacket.h
	$(CC) $(C_FLAGS) -c ../CANPacket.c -o build/CANPacket.o

Port.o: $(BUILD) ../PortFiles/PortJetson.c ../Port.h
	$(CC) $(C_FLAGS) -DCHIP_TYPE=CHIP_TYPE_JETSON -c ../PortFiles/PortJetson.c -o build/Port.o

CANCommon.o: $(BUILD) ../CANCommon.c ../CANCommon.h
	$(CC) $(C_FLAGS) -c ../CANCommon.c -o build/CANCommon.o

unity.o: $(BUILD) unity/unity.c unity/unity.h
	$(CC) $(C_FLAGS) -c unity/unity.c -o build/unity.o

# Compile each test file with Unity
$(BUILD)/%: %.c $(UNITY_SRC) | $(BUILD)
	$(CC) $(C_FLAGS) $< unity.c -o $@

$(BUILD):
	mkdir -p $(BUILD)

out:
	mkdir -p out

clean:
	rm -rf build/

.PHONY: all clean run_tests
