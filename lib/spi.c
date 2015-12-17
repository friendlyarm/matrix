#include "common.h"
#include "includes/spidev.h"
#include "includes/spi_enum.h"

#define SPI_MAX_SPEED 25000000

EXPORT int setSPIWriteBitsPerWord(int spi_fd, int bits) 
{
    clearLastError();
	int ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_WR_BITS_PER_WORD");
	}
	return ret;
}

EXPORT int setSPIReadBitsPerWord(int spi_fd, int bits) 
{
	int ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    clearLastError();
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_RD_BITS_PER_WORD");
	}
	return ret;
}

EXPORT int setSPIBitOrder(int spi_fd, int order) 
{
	int ret;
	int spi_mode = 0;
    clearLastError();
	if(order == LSBFIRST) {
		spi_mode |=  SPI_LSB_FIRST;
	} else {
		spi_mode &= ~SPI_LSB_FIRST;
	}
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_WR_MODE");
		return ret;
	}
	return ret;
}

EXPORT int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed) 
{
	int ret;
	unsigned int realSpeed;
    clearLastError();
    if (spi_speed<0 || spi_speed>SPI_MAX_SPEED) {
        setLastError("invalid spi speed %d", spi_speed);
    }
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_WR_MAX_SPEED_HZ");
		return ret;
	}
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &realSpeed);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_RD_MAX_SPEED_HZ");
		return ret;
	}
	return ret;
}

EXPORT int setSPIDataMode(int spi_fd, int mode)
{
	int ret;
	int spi_mode = 0;
    clearLastError();
	switch(mode) {
		case SPI_MODE0:
			spi_mode &= ~(SPI_CPHA|SPI_CPOL);
			break;
		case SPI_MODE1:
			spi_mode &= ~(SPI_CPOL);
			spi_mode |= (SPI_CPHA);
			break;
		case SPI_MODE2:
			spi_mode |= (SPI_CPOL);
			spi_mode &= ~(SPI_CPHA);
			break;
		case SPI_MODE3:
			spi_mode |= (SPI_CPHA|SPI_CPOL);
			break;
		default:
			setLastError("error SPIDataMode");
			return -1;
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_WR_MODE");
		return ret;
	}

	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &mode);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_RD_MODE");
		return ret;
	}

	return ret;
}

/*
 struct spi_ioc_transfer {
 __u64               tx_buf;                       // 写数据缓冲
 __u64               rx_buf;                       // 读数据缓冲
 __u32               len;                          // 缓冲的长度
 __u32               speed_hz;                     // 通信的时钟频率
 __u16               delay_usecs;                  // 两个spi_ioc_transfer之间的延时
 __u8                 bits_per_word;               // 字长（比特数）
 __u8                 cs_change;                   // 是否改变片选
 __u32               pad;
 };
 */

EXPORT int SPItransferOneByte(
	   int spi_fd
	   , unsigned char byteData
	   , int spi_delay
	   , int spi_speed
	   , int spi_bits
	   ) 
{
	int ret;
	unsigned char tx[1] = {0};
	unsigned char rx[1] = {0};
	tx[0] = byteData;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = 1;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

    clearLastError();
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_MESSAGE");
		return ret;
	}
	return rx[0];
}

EXPORT int SPItransferBytes(
	 int spi_fd
	 , unsigned char * writeData
	 , int writeLen
	 , unsigned char * readBuffer
	 , int readLen
	 , int spi_delay
	 , int spi_speed
	 , int spi_bits
	 )
{
	unsigned int len = writeLen;
	if (len > readLen) {
		len = readLen;
	}

	unsigned char * pWriteData = writeData;
	unsigned char * pReadBuffer = readBuffer;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)pWriteData;
	tr.rx_buf = (unsigned long)pReadBuffer;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);

    clearLastError();
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}

EXPORT int writeBytesToSPI(
		int spi_fd
		, unsigned char * writeData
		, int writeLen
		, int spi_delay
		, int spi_speed
		, int spi_bits
		) 
{
	unsigned int len = writeLen;

	unsigned char * pWriteData = writeData;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)pWriteData;
	tr.rx_buf = (unsigned long)0;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    clearLastError();
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}

EXPORT int readBytesFromSPI(
	 int spi_fd
	 , unsigned char * readBuffer
	 , int readLen
	 , int spi_delay
	 , int spi_speed
	 , int spi_bits
	 ) 
{
	unsigned int len = readLen;

	unsigned char * pReadBuffer = readBuffer;

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)0;
	tr.rx_buf = (unsigned long)pReadBuffer;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    clearLastError();
	if (ret < 0) {
		setLastError("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}
