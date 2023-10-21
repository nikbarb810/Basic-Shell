# makefile for your C program

CC = gcc
CFLAGS = -Wall
TARGET = myshell

all: $(TARGET)

$(TARGET): main.c main.h
	$(CC) $(CFLAGS) -o $(TARGET) main.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
