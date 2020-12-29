CC = gcc
CFLAGS = -g -Wall -std=c11

test: CANTest.o CANPacket.o Port.o
	$(CC) $(CFLAGS) -o test CANTest.o CANPacket.o Port.o

CANTest.o: CANTest.c CANPacket.h CANTest.h
	$(CC) $(CFLAGS) -o CANTest.o -c CANTest.c

CANPacket.o: CANPacket.c CANPacket.h Port.h
	$(CC) $(CFLAGS) -o CANPacket.o -c CANPacket.c

Port.o: ./PortFiles/PortUnitTest.c Port.h
	$(CC) $(CFLAGS) -o Port.o -c ./PortFiles/PortUnitTest.c
