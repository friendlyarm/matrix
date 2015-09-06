CROSS_COMPILE ?= arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc 

TARGET = $(shell basename `pwd`) 
SRC =  $(wildcard *.c)
SRC +=  $(wildcard *.C)

$(TARGET):$(SRC)
	$(CC) -o $@ -L ../../lib/ -I ../../lib/includes/ $^ -lfahw -lm -Wall

.PHONY: clean 
clean:
	rm -f $(TARGET)
