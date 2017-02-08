#!/bin/bash

if [ $UID -ne 0 ]
    then
    echo "Please run as root."
    exit 1
fi

if [ $# -ne 1 ]; then
    echo "Usage:${0} /dev/fbx"
    exit 1
fi
FB_DEV=${1}

HARDWARE=`cat /proc/cpuinfo | grep Hardware`
REVISION=`cat /proc/cpuinfo | grep Revision`
HARDWARE=${HARDWARE#*: }
REVISION=${REVISION#*: }
echo "Hardware:${HARDWARE}"
echo "Revision:${REVISION}"

if [[ "x${FB_DEV}" = "x/dev/fb-st7735s" ]]; then
    if [[ "x${HARDWARE}" = "xsun8i" ]] && [[ "x${REVISION}" = "x0000" ]]; then      # nanopi-m1/m1+/neo/air
        modprobe fbtft_device name=matrix-st7735s gpios=dc:17,reset:3,cs:201
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]]; then                                    # pi2/fire/m2
        modprobe fbtft_device name=matrix-st7735s gpios=dc:58,reset:63,cs:59        
    elif [[ "x${HARDWARE}" = "xNANOPI3" ]]; then                                    # m3
        modprobe fbtft_device name=matrix-st7735s gpios=dc:58,reset:63,cs:59
    else
        echo "Unsupported board"
        exit 1
    fi
    export QWS_DISPLAY=Transformed:Rot90:Linuxfb:${FB_DEV}:enable=1:mWidth90:mmHeight45:0
elif [[ "x${FB_DEV}" = "x/dev/fb-st7789s" ]]; then
    if [[ "x${HARDWARE}" = "xsun8i" ]] && [[ "x${REVISION}" = "x0000" ]]; then      # nanopi-m1/m1+/neo/air
        modprobe fbtft_device name=matrix-st7789s gpios=dc:1,reset:203,cs:67
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]] && [[ "x${REVISION}" = "x0000" ]]; then  # nanopi 2
        modprobe fbtft_device name=matrix-st7789s gpios=dc:97,reset:60,cs:94        
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]] && [[ "x${REVISION}" = "x0004" ]]; then  # nanopi fire
        modprobe fbtft_device name=matrix-st7789s gpios=dc:60,reset:104,cs:94        
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]] && [[ "x${REVISION}" = "x0005" ]]; then  # nanopi m2
        modprobe fbtft_device name=matrix-st7789s gpios=dc:60,reset:104,cs:94        
    elif [[ "x${HARDWARE}" = "xNANOPI3" ]] && [[ "x${REVISION}" = "x0005" ]]; then  # nanopi m3
        modprobe fbtft_device name=matrix-st7789s gpios=dc:60,reset:104,cs:94
    else
        echo "Unsupported board"
        exit 1
    fi
    export QWS_DISPLAY=Transformed:Rot90:Linuxfb:${FB_DEV}:enable=1:mWidth120:mmHeight92:0
else
    # 16:9
    export QWS_DISPLAY=Transformed:Rot0:Linuxfb:${FB_DEV}:enable=1:mWidth400:mmHeight225:0
fi

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export PATH=/bin:/sbin:/usr/bin/:/usr/sbin:/usr/local/bin
export QWS_MOUSE_PROTO="MouseMan:/dev/input/mice"
export QWS_KEYBOARD=TTY:/dev/tty1

killall nanopi-status > /dev/null 2>&1
./nanopi-status -qws &
