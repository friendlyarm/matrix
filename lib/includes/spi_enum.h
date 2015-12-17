#ifndef __SPI_ENUM__
#define __SPI_ENUM__

/// SPIBitOrder
/// Specifies the SPI data bit ordering
typedef enum
{
	LSBFIRST = 0,  ///< LSB First
	MSBFIRST = 1   ///< MSB First
}SPIBitOrder;

/// SPIMode
/// Specify the SPI data mode
typedef enum
{
	SPI_MODE0 = 0,  ///< CPOL = 0, CPHA = 0
	SPI_MODE1 = 1,  ///< CPOL = 0, CPHA = 1
	SPI_MODE2 = 2,  ///< CPOL = 1, CPHA = 0
	SPI_MODE3 = 3,  ///< CPOL = 1, CPHA = 1
}SPIMode;

/// SPIClockDivider
/// Specifies the divider used to generate the SPI clock from the system clock.
/// Figures below give the divider, clock period and clock frequency.
typedef enum
{
	SPI_CLOCK_DIV65536 = 0,       ///< 65536 = 256us = 4kHz
	SPI_CLOCK_DIV32768 = 32768,   ///< 32768 = 126us = 8kHz
	SPI_CLOCK_DIV16384 = 16384,   ///< 16384 = 64us = 15.625kHz
	SPI_CLOCK_DIV8192  = 8192,    ///< 8192 = 32us = 31.25kHz
	SPI_CLOCK_DIV4096  = 4096,    ///< 4096 = 16us = 62.5kHz
	SPI_CLOCK_DIV2048  = 2048,    ///< 2048 = 8us = 125kHz
	SPI_CLOCK_DIV1024  = 1024,    ///< 1024 = 4us = 250kHz
	SPI_CLOCK_DIV512   = 512,     ///< 512 = 2us = 500kHz
	SPI_CLOCK_DIV256   = 256,     ///< 256 = 1us = 1MHz
	SPI_CLOCK_DIV128   = 128,     ///< 128 = 500ns = = 2MHz
	SPI_CLOCK_DIV64    = 64,      ///< 64 = 250ns = 4MHz
	SPI_CLOCK_DIV32    = 32,      ///< 32 = 125ns = 8MHz
	SPI_CLOCK_DIV16    = 16,      ///< 16 = 50ns = 20MHz
	SPI_CLOCK_DIV8     = 8,       ///< 8 = 25ns = 40MHz
	SPI_CLOCK_DIV4     = 4,       ///< 4 = 12.5ns 80MHz
	SPI_CLOCK_DIV2     = 2,       ///< 2 = 6.25ns = 160MHz
	SPI_CLOCK_DIV1     = 1,       ///< 0 = 256us = 4kHz
} SPIClockDivider;

#endif