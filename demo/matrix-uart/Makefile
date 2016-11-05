CC = gcc 

TARGET	= commtest 
SRC 	= commtest.c

$(TARGET):$(SRC)
	$(CC) -o $@  $^ -REENTRANT -Wall -lfahw -lm -lpthread 

.PHONY: clean 
clean:
	rm -f $(TARGET)
