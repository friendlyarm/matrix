#define _GNU_SOURCE
#include "common.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

EXPORT int openHW(const char *devName,int flags) {
    clearLastError();
    int fd = -1;
    const char *strDevName = devName;
    fd = open(strDevName, flags);
    if (fd < 0) {
        setLastError("Open %s error: %s(%d)",devName, strerror(errno),errno);
    }

    return fd;
}

EXPORT int writeHW(int fd, const void *_data, size_t len) {
    clearLastError();
    if (len <= 0) {
        return 0;
    }
    unsigned char *pData = (unsigned char *)_data;
    int ret = write(fd, pData, len);
    if (ret < 0) {
        setLastError("Write file error: %s(%d)",strerror(errno),errno);
    }
    return ret;
}

EXPORT int readHW(int fd, void *_data, size_t len) {
    clearLastError();
    if (len <= 0) {
        return 0;
    }

    int retRead = 0;
    unsigned char* pRetData = _data;
    if (pRetData) {
        retRead = read(fd, pRetData, len);
    }
    if (retRead < 0) {
        setLastError("Read file error: %s(%d)",strerror(errno),errno);
    }
    return retRead;
}

EXPORT int selectHW(int fd, int sec, int usec) {
    clearLastError();
    fd_set rfds;
    struct timeval timeout;
    int ret = -1;
    
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    
    if ( sec==0 && usec== 0 ) {
    	ret = select(fd + 1, &rfds, NULL, NULL, NULL);
    } else {    
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;
		ret = select(fd + 1, &rfds, NULL, NULL, &timeout);
    }
    
    if (ret < 0) {
    	setLastError("Fail to select");
        return -1;
    }

    if (FD_ISSET(fd,&rfds)) {
        return 1;
    }

    return 0;
}

EXPORT void closeHW(int fd) {
    clearLastError();
    close(fd);
}

EXPORT int ioctlWithIntValue(int fd, int cmd, int value) {
    clearLastError();
    return ioctl(fd, cmd, &value);
}
