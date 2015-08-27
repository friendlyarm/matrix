#include "common.h"

EXPORT int getLedState(int ledID) {
    clearLastError();
	char ledPath[64]; 
	int ledVal = -1;
	sprintf(ledPath, "/sys/class/leds/led%d/brightness", ledID);
		
	if (ledID>=0 && ledID<=4) {
		ledVal = readIntValueFromFile(ledPath);
	    if (ledVal > 0) {	        
	    	ledVal = 1;
	    } else {
	    	ledVal = 0;
	    } 
	} else {
	    setLastError("Unsupport led %d", ledID);
	}

	return ledVal;
}

EXPORT int setLedState(int ledID, int ledState) {
    clearLastError();
    int ret = -1;
    char ledPath[64]; 
    sprintf(ledPath, "/sys/class/leds/led%d/brightness", ledID);
    
    if (ledID>=0 && ledID<=4) {
    	ret = writeIntValueToFile(ledPath, ledState);
    } else {
    	setLastError("Unsupport led %d", ledID);
    }
    return ret;
}

EXPORT int setLedStateForTiny4412(int ledID, int ledState) {
    clearLastError();
	int fd = open("/dev/leds0", O_RDONLY);
	if (fd < 0) {
		fd = open("/dev/leds", O_RDONLY);
	}
	if (fd < 0) {
		setLastError("Can't open led device!");
		return -1;
	}

	if ((ledState == 0 || ledState == 1) && (ledID >= 0 && ledID <= 3)) {
		if (ioctl(fd, ledState, ledID) == -1) {
			setLastError("set LED state error!");
			goto ERROR;
		}
	} else {
		setLastError("Wrong args!");
		goto ERROR;
	}

	close(fd);
	return 0;

ERROR: 
	close(fd);
	return -1;
}
