#include <errno.h>
#include <termio.h>
#include <features.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <string.h>
#include <paths.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/types.h>
#include "common.h"
#include "libfahw-filectl.h"
#include "libfahw-w1.h"

static void rTrim(char *sSource)
{
    char *sPtr;
    if(sSource==NULL)
        return;
    sPtr = sSource + strlen(sSource) - 1;

    while(sPtr>=sSource && (isspace(*sPtr)))
        sPtr--;
    *(sPtr+1)=0;
}

/*
f6 01 4b 46 7f ff 0a 10 eb : crc=eb YES
f6 01 4b 46 7f ff 0a 10 eb t=31375
 */
static int readTemp(const char* filename, int* ok) 
{
    FILE* f = fopen(filename, "r");

    char line[512];
    int chIndex;
    char ch;
    int len;
    int result = 0;

    *ok = 0;
    if (f != NULL) {
        chIndex = 0;
        line[0] = 0;
        int lineNum = 1;
        int yes = 0;

        while(1) {
            if (fread(&ch, 1, 1, f) == 1) {
                if (ch == '\n') {
                    line[chIndex] = 0;
                    chIndex=0;
                    rTrim(line);
                    len = strlen(line);

                    if (len > 0) {
                        // printf("line[%d] %s\n", lineNum, line);
                        if (lineNum == 1) {
                            if (line[strlen(line)-1] == 'S'
                                    && line[strlen(line)-2] == 'E'
                                            && line[strlen(line)-3] == 'Y'
                            ) {
                                yes = 1;
                            }
                        } else if (yes == 1 && lineNum == 2) {
                            char* pLine = strstr(line, "t=");
                            if (pLine != 0) {
                                char* pTemp = pLine + 2;
                                result = atoi(pTemp);
                                // printf("pTemp: %s\n", pTemp);
                                *ok = 1;
                            }
                        }

                        lineNum ++;
                    }

                } else {
                    line[chIndex++] = ch;
                }
            } else {
                break;
            }
        }

        line[chIndex] = 0;
        chIndex=0;
        rTrim(line);
        len = strlen(line);
        if (len > 0) {
            if (yes == 1 && lineNum == 2) {
                char* pLine = strstr(line, "t=");
                if (pLine != 0) {
                    char* pTemp = pLine + 2;
                    result = atoi(pTemp);
                    printf("pTemp: %s\n", pTemp);
                    *ok = 1;
                }
            }
        }

        fclose(f);
    }
    return result;
}

EXPORT int ds18b20Read(char * temperature) 
{
    DIR *d;
    struct dirent *de;
    clearLastError();
    if (!(d = opendir(DS18B20_SYS_PATH))) {
        setLastError("Fail to opendir %s", DS18B20_SYS_PATH);
        return 0;
    }

    int ok = 0;
    int temp = 0;
    char fname[255];
    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;

        sprintf( fname, "%s%s/w1_slave", DS18B20_SYS_PATH, de->d_name);
        if( access( fname, F_OK ) != -1 ) {
            temp = readTemp(fname, &ok);
            break;
        }
    }
    closedir(d);

    if (ok == 0) {
        setLastError("Fail to read %s", fname);
        return 0;
    }

    char* pRetData = temperature;
    if (pRetData) {
        sprintf(pRetData, "%d", temp);
    }
    int retSize = strlen(pRetData);
    return retSize;
}
