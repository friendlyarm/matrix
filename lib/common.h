 #ifndef _COMMON_H_
 #define _COMMON_H_

#define __DEBUG

#ifdef __DEBUG
    #define DEBUG(format, args...) \
        printf("FAHW-Lib: " format, ## args)
#else
    #define DEBUG(format, args...)
#endif

#include <string.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <linux/fs.h> 
#include <errno.h> 
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

extern int boardInit();
extern void clearLastError();
extern void setLastError(const char *fmt, ...);
extern int getLastError(char* dest, int maxlen);

#define EXPORT
extern int writeValueToFile(char* fileName, char* buff);
extern int writeIntValueToFile(char* fileName, int value);
extern int readValueFromFile(char* fileName, char* buff, int len);
extern int readIntValueFromFile(char* fileName);

#define FILE_PATH_LENGTH           (128)
#define BOARD_MINI6410             (6410)
#define BOARD_MINI210              (210)
#define BOARD_TINY4412             (4412)
#define BOARD_NANOPI_M1            (68330)  //'H3'
#define BOARD_NANOPI_2             (44180)
#define BOARD_NANOPI_2_FIRE        (44184)
#define BOARD_NANOPI_M2            (44185)
#define BOARD_NANOPC_T2            (44181)
#define BOARD_NANOPI_M3		   	   (68187)
#define BOARD_NANOPC_T3		   	   (68181)
#endif
