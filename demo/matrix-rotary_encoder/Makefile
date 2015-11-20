CROSS_COMPILE ?= arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc 

TARGET	=  $(patsubst Matrix-%.c,matrix-%, $(wildcard *.c))
SRC 	=  $(wildcard *.c)
SRC 	+= $(wildcard *.h)

$(TARGET):$(SRC)
	$(CC) -o $@ -L ../../lib/ -I ../../lib/includes/ $^ -lfahw -lm -Wall

.PHONY: clean 
clean:
	rm -f $(TARGET)
