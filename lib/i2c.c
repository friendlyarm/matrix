#include "common.h"
#include "includes/i2c-dev.h"
    
EXPORT int setI2CSlave(int fd, int slave) {
    clearLastError();
	if (ioctl(fd, I2C_SLAVE, slave) < 0) {
		setLastError("Fail ioctl I2C_SLAVE");
		return -1;
	}

	return 0;
}

EXPORT int setI2CTimeout(int fd, int timeout) {
    clearLastError();
	if (ioctl(fd, I2C_TIMEOUT, timeout) < 0) {
		setLastError("Fail ioctl I2C_TIMEOUT");
		return -1;
	}

	return 0;
}

EXPORT int setI2CRetries(int fd, int retries) {
    clearLastError();
	if (ioctl(fd, I2C_RETRIES, retries) < 0) {
		setLastError("Fail ioctl I2C_RETRIES");
		return -1;
	}

	return 0;
}

EXPORT int I2CWriteByteTo(int fd, int pos, unsigned char byteData, int wait_ms) {
    clearLastError();
	if (i2c_smbus_write_byte_data(fd, (unsigned char)pos, (unsigned char)byteData) < 0) {
		setLastError("Fail to I2CWriteByteTo");
		return -1;
	}
	usleep(1000*wait_ms);

	return 0;
}

EXPORT int I2CReadByteFrom(int fd, int pos, int wait_ms) {
    clearLastError();
	ioctl(fd, BLKFLSBUF); // clear kernel read buffer
	if (i2c_smbus_write_byte(fd, (unsigned char)pos) < 0) {
		setLastError("Fail to I2CReadByteFrom");
		return -1;
	}
	usleep(1000*wait_ms);
	return i2c_smbus_read_byte(fd);
}

EXPORT int I2CWriteByte(int fd, unsigned char byteData, int wait_ms) {
    clearLastError();
	if (i2c_smbus_write_byte(fd, (unsigned char)byteData) < 0) {
		setLastError("Fail to I2CWriteByte");
		return -1;
	}
	usleep(1000*wait_ms);

	return 0;
}

EXPORT int I2CReadByte(int fd, int wait_ms) {
    clearLastError();
    int ret;
	ioctl(fd, BLKFLSBUF); // clear kernel read buffer
	usleep(1000*wait_ms);
	if ((ret = i2c_smbus_read_byte(fd)) == -1) {
	    setLastError("Fail to I2CReadByte");
	}
	return ret;
}
