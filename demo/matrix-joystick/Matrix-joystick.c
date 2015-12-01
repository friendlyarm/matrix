#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "libfahw.h"

#define PS2_READ_TIMES      (10000)

int main(int argc, char ** argv)
{
    int i = 0;
    int x, y;

    x = y = 0;
    for (i=0; i<PS2_READ_TIMES; i++) {
        if (pcf8591Read(1, &x) != -1 && pcf8591Read(2, &y) != -1) {
            printf("x=%4d y=%4d\n", x, y);
        }
    }
    return 0;
}
