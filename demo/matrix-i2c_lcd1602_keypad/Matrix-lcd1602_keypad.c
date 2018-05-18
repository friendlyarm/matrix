#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
#include <sys/socket.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "libfahw.h"

#define TIME_STR_BUFSIZE     32

int showIP(int devFD, char *netDev)
{
    int socketFD;
    struct sockaddr_in *sin;
    struct ifreq ifr;
    socketFD = socket(AF_INET, SOCK_DGRAM, 0); 
    if(socketFD == -1) {
        printf("socket error\n");
        return -1;
    }
    strcpy(ifr.ifr_name, netDev);
    if(ioctl(socketFD, SIOCGIFADDR, &ifr) < 0) {
        return -1;
    } else {
        sin = (struct sockaddr_in *)&(ifr.ifr_addr);
        LCD1602KeyDispStr(devFD, 0, 1, inet_ntoa(sin->sin_addr));
    }
    return 0;
}

int main(int argc, char ** argv)
{
    int devFD, board;
    int keyValue = 0;
    int lastKeyValue = -1;
    int showDefault = 1;
    int needClear = 1;
    time_t lt;
    char curTime[TIME_STR_BUFSIZE];
    char preTime[TIME_STR_BUFSIZE];
    int i2cDev = 0;
    
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
    
    if (argc == 2)
        i2cDev = atoi(argv[1]);    
    if ((devFD = LCD1602KeyInit(i2cDev)) == -1) {
        printf("Fail to init LCD1602\n");
        return -1;
    }
    LCD1602KeyClear(devFD);
    printf("waiting key press...\n");
    while (1) {
        keyValue = LCD1602GetKey(devFD);
		keyValue &= 0x1f;
        if (keyValue != lastKeyValue) {
            lastKeyValue = keyValue;
        } else if (showDefault != 1){
            usleep(1000);
            continue;
        }
        switch (keyValue) {
        // F1
        case 0x1e:
            showDefault = 0;
            LCD1602KeyClear(devFD);
            LCD1602KeyDispStr(devFD, 0, 0, "#F1-IP address");
            if (showIP(devFD, "eth0")) {
            	if (showIP(devFD, "wlan0")) {
                	if (showIP(devFD, "usb0")) {
                	    showIP(devFD, "lo");
                	}
                }
            }
            break;
            // F2    
        case 0x1d:
            showDefault = 0;
            LCD1602KeyClear(devFD);
            LCD1602KeyDispStr(devFD, 0, 0, "#F2-Your favor");
            LCD1602KeyDispStr(devFD, 0, 1, "Come add it");
            break;
            // F3    
        case 0x1b:
            showDefault = 0;
            LCD1602KeyClear(devFD);
            LCD1602KeyDispStr(devFD, 0, 0, "#F3-Your idea");
            LCD1602KeyDispStr(devFD, 0, 1, "Come show it");
            break;
            // F4
        case 0x17:
            showDefault = 0;
            LCD1602KeyClear(devFD);
            LCD1602KeyDispStr(devFD, 0, 0, "#F4-About");
            LCD1602KeyDispStr(devFD, 0, 1, "by FriendlyARM");
            break;
            // F5
        case 0xf:
            showDefault = 1;
            break;
        }
        if (showDefault == 1) {
            if (needClear) {
                LCD1602KeyClear(devFD);
                LCD1602KeyDispStr(devFD, 0, 0, "#Default");
                needClear = 0;
            }
            memset(curTime, 0, TIME_STR_BUFSIZE);
            lt = time(NULL);
            strncpy(curTime, ctime(&lt) + 11, 8);
            if(strcmp(curTime, preTime)) {
                printf("time:%s\n", curTime);
                LCD1602KeyDispStr(devFD, 0, 1, curTime);
            }
            memset(preTime, 0, TIME_STR_BUFSIZE);
            strcpy(preTime, curTime);
        } else {
            needClear = 1;
            usleep(1000);
        }
    }    
    printf("quit reading key press\n");
    LCD1602KeyDeInit(devFD);
    return 0;
}









