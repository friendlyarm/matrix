#!/bin/bash
#set -x
if [ $UID -ne 0 ]
    then
    echo "Please run as root"
    exit 1
fi

if [ $# -ne 1 ]; then
    echo "Usage:${0} /dev/fbx"
    exit 1
fi
FB_DEV=${1}
TS_DEV="/dev/input/ts-ads7846"

HARDWARE=`cat /proc/cpuinfo | grep Hardware`
REVISION=`cat /proc/cpuinfo | grep Revision`
HARDWARE=${HARDWARE#*: }
REVISION=${REVISION#*: }
echo "Hardware:${HARDWARE}"
echo "Revision:${REVISION}"

# clean up
rm -rf ./Xorg.log
if fuser ${TS_DEV} >/dev/null 2>&1; then
    # echo "clean up ts"
    killall -9 xinput_calibrator >/dev/null 2>&1
    sleep 1
    FUSER_RESULT=`fuser ${TS_DEV}`
    for word in ${FUSER_RESULT}
    do 
        if [[ ${word} =~ [0-9]+ ]]; then
            kill ${word}
        fi
    done
    sleep 1
fi
if fuser ${FB_DEV} >/dev/null 2>&1; then
    # echo "clean up fb"
    FUSER_RESULT=`fuser ${FB_DEV}`
    for word in ${FUSER_RESULT}
    do 
        if [[ ${word} =~ [0-9]+ ]]; then
            kill ${word}
        fi
    done
    killall -9 startx >/dev/null 2>&1
    sleep 1
fi
rmmod matrix_ads7846 fbtft_device >/dev/null 2>&1

if [[ "x${FB_DEV}" = "x/dev/fb-st7735s" ]]; then
    if [[ "x${HARDWARE}" = "xsun8i" ]] && [[ "x${REVISION}" = "x0000" ]]; then		# nanopi-m1/m1+/neo/air
		modprobe fbtft_device name=matrix-st7735s gpios=dc:17,reset:3,cs:201
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]]; then	                                # pi2/fire/m2
        modprobe fbtft_device name=matrix-st7735s gpios=dc:58,reset:63,cs:59        
    elif [[ "x${HARDWARE}" = "xNANOPI3" ]]; then                                    # m3
        modprobe fbtft_device name=matrix-st7735s gpios=dc:58,reset:63,cs:59
    else
		echo "Unsupported board"
		exit 1
	fi
elif [[ "x${FB_DEV}" = "x/dev/fb-st7789s" ]]; then
    if [[ "x${HARDWARE}" = "xsun8i" ]] && [[ "x${REVISION}" = "x0000" ]]; then      # nanopi-m1/m1+/neo/air
        modprobe fbtft_device name=matrix-st7789s gpios=dc:1,reset:203,cs:67
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]] && [[ "x${REVISION}" = "x0000" ]]; then	# nanopi 2
        modprobe fbtft_device name=matrix-st7789s gpios=dc:97,reset:60,cs:94        
    elif [[ "x${HARDWARE}" = "xNANOPI2" ]] && [[ "x${REVISION}" = "x0002" ]]; then  # nanopi s2
        modprobe fbtft_device name=matrix-st7789s gpios=dc:60,reset:104,cs:94
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
fi
FRAMEBUFFER=${FB_DEV} startx >Xorg.log 2>&1 &

if [[ "x${HARDWARE}" != "xsun8i" ]]; then                   # nanopi-m1 not support touchscreen yet
    let TIMEOUT=30
    while [ ${TIMEOUT} -gt 0 ]; do
        sleep 2
        let TIMEOUT-=2
        DISPLAY_ENV=`grep -oe "Xorg.[0-9]" ./Xorg.log`
        DISPLAY_ENV=${DISPLAY_ENV#*.}
        if  [ ! -n "$DISPLAY_ENV" ] ;then
            printf "Waiting Desktop...%2d\r" ${TIMEOUT}
        else
            echo ""
            break
        fi
    done
    echo "DISPLAY_ENV=${DISPLAY_ENV}"

    modprobe matrix_ads7846
    DISPLAY=:${DISPLAY_ENV}.0 xinput_calibrator --device "ADS7846 Touchscreen" &
fi
