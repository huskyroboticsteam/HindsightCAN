CC = g++
C_FLAGS = -g -Wall -std=c++17

BUILD = build

SRC_DIRS := .

SRC_FILES := $(foreach dir, $(TARGET_DIRS), $(wildcard $(dir)/*.c))

OBJ := $(patsubst %.c,$(BUILD)/%.o,$(filter %.c,$(SRC_FILES)))

all: program

debug: debug_program

program: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o run $(PA) -pthread

$(BUILD)/%.o: %.c
	$(CC) $(CFLAGS) $(PA) -c $< -o $@

$(BUILD_DIRS):
	mkdir -p $@

data: data.o
	$(CC) $(CFLAGS) -o $(BUILD)/data $(BUILD)/data.o

unity.o: $(TST)/unity/unity.c $(TST)/unity/unity.h | $(BUILD_DIRS)
	$(CC) $(CFLAGS) -c $(TST)/unity/unity.c -o $(BUILD)/unity.o

data_test: unity.o $(TST)/data_tests.cpp $(BUILD)/lib/data.o
	$(CC) $(CFLAGS) -o $(TST)/data_tests $(BUILD)/unity.o $(TST)/data_tests.cpp lib/data.cpp

run_test: data_test ctrl_test
	$(TST)/data_tests

clean:
	rm -rf build/

.PHONY: all clean