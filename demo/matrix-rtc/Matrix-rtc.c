#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "libfahw.h"

#define DRIVER_MODULE        "rtc-ds1307"
static const char default_rtc[] = "/dev/rtc-ds1307";
static const char default_date_time[] = "2015 9 15 1 1 1";

int main(int argc, char **argv)
{
    int fd, retval, board;
    struct rtc_time rtc_tm;
    const char *rtc = default_rtc;
    const char *date_time = default_date_time;

#if 0
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
#endif
    
    switch (argc) {
    case 3:
        rtc = argv[1];
        date_time = argv[2];
        break;
    case 1:
        break;
    default:
        fprintf(stderr, "usage:  rtctest [rtcdev] [year mon day hour min sec]\n");
        return 1;
    }
    system("modprobe "DRIVER_MODULE);
    sleep(1);
    fd = open(rtc, O_RDONLY);
    if (fd ==  -1) {
        perror(rtc);
        goto err;
    }
    fprintf(stderr, "RTC Driver Test Example.\n");

    sscanf(date_time, "%d %d %d %d %d %d",
            &rtc_tm.tm_year,
            &rtc_tm.tm_mon,
            &rtc_tm.tm_mday,
            &rtc_tm.tm_hour,
            &rtc_tm.tm_min,
            &rtc_tm.tm_sec); 
    rtc_tm.tm_year -= 1900;
    rtc_tm.tm_mon -= 1;
    retval = ioctl(fd, RTC_SET_TIME, &rtc_tm);
    if (retval == -1) {
        perror("RTC_SET_TIME ioctl");
        goto err;
    }

    fprintf(stderr, "Set RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
            rtc_tm.tm_mon + 1, rtc_tm.tm_mday, rtc_tm.tm_year + 1900,
            rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

    /* Read the RTC time/date */
    retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
    if (retval == -1) {
        perror("RTC_RD_TIME ioctl");
        goto err;
    }

    fprintf(stderr, "Read RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
            rtc_tm.tm_mon + 1, rtc_tm.tm_mday, rtc_tm.tm_year + 1900,
            rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

    fprintf(stderr, "Test complete\n");
    close(fd);
err:
    system("rmmod "DRIVER_MODULE);
    return 0;
}
