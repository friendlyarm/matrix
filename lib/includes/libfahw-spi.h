#ifndef __FRIENDLYARM_HARDWARE_SPI_H__
#define __FRIENDLYARM_HARDWARE_SPI_H__


// SPIBitOrder
#define LSBFIRST  (0)  ///< LSB First
#define MSBFIRST  (1)   ///< MSB First

// SPIMode
#define SPI_MODE0  (0)  ///< CPOL  (0, CPHA  (0
#define SPI_MODE1  (1)  ///< CPOL  (0, CPHA  (1
#define SPI_MODE2  (2)  ///< CPOL  (1, CPHA  (0
#define SPI_MODE3  (3)  ///< CPOL  (1, CPHA  (1
	
	
#define SPI_CPHA  (0x01)
#define SPI_CPOL  (0x02)
#define SPI_CS_HIGH  (0x04)
#define SPI_LSB_FIRST  (0x08)
#define SPI_3WIRE  (0x10)
#define SPI_LOOP  (0x20)
#define SPI_NO_CS  (0x40)
#define SPI_READY  (0x80)

// SPIClockDivider
#define SPI_CLOCK_DIV65536  (0)       ///< 65536  (256us  (4kHz
#define SPI_CLOCK_DIV32768  (32768)   ///< 32768  (126us  (8kHz
#define SPI_CLOCK_DIV16384  (16384)   ///< 16384  (64us  (15.625kHz
#define SPI_CLOCK_DIV8192   (8192)    ///< 8192  (32us  (31.25kHz
#define SPI_CLOCK_DIV4096   (4096)    ///< 4096  (16us  (62.5kHz
#define SPI_CLOCK_DIV2048   (2048)    ///< 2048  (8us  (125kHz
#define SPI_CLOCK_DIV1024   (1024)    ///< 1024  (4us  (250kHz
#define SPI_CLOCK_DIV512    (512)     ///< 512  (2us  (500kHz
#define SPI_CLOCK_DIV256    (256)     ///< 256  (1us  (1MHz
#define SPI_CLOCK_DIV128    (128)     ///< 128  (500ns  (= 2MHz
#define SPI_CLOCK_DIV64     (64)      ///< 64  (250ns  (4MHz
#define SPI_CLOCK_DIV32     (32)      ///< 32  (125ns  (8MHz
#define SPI_CLOCK_DIV16     (16)      ///< 16  (50ns  (20MHz
#define SPI_CLOCK_DIV8      (8)       ///< 8  (25ns  (40MHz
#define SPI_CLOCK_DIV4      (4)       ///< 4  (12.5ns 80MHz
#define SPI_CLOCK_DIV2      (2)       ///< 2  (6.25ns  (160MHz
#define SPI_CLOCK_DIV1      (1)       ///< 0  (256us  (4kHz


int setSPIWriteBitsPerWord( int spi_fd, int bits );
int setSPIReadBitsPerWord( int spi_fd, int bits );
int setSPIBitOrder( int spi_fd, int order);
int setSPIMaxSpeed(int spi_fd, unsigned int spi_speed);
int setSPIDataMode( int spi_fd, int mode);
int SPItransferOneByte( int spi_fd, unsigned char byteData, int spi_delay, int spi_speed, int spi_bits);
int SPItransferBytes(
         int spi_fd
         , unsigned char * writeData
         , int writeLen
         , unsigned char * readBuffer
         , int readLen
         , int spi_delay
         , int spi_speed
         , int spi_bits
         );    
int writeBytesToSPI(
         int spi_fd
         , unsigned char * writeData
         , int writeLen
         , int spi_delay
         , int spi_speed
         , int spi_bits
         );
int readBytesFromSPI(
         int spi_fd
         , unsigned char * readBuffer
         , int readLen
         , int spi_delay
         , int spi_speed
         , int spi_bits
         );


#define SPI0_PATH "/dev/spidev0.0"

#endif
