#ifndef __FRIENDLYARM_HARDWARE_ENCODER_H__
#define __FRIENDLYARM_HARDWARE_ENCODER_H__

#define ENCODER_SW                  (1)
#define ENCODER_VALUE               (2)
#define ENCODER_PATH                "/sys/devices/platform/rotary_encoder/"

int rotaryEncoderInit(int swPin, int siaPin, int sibPin);
int rotaryEncoderRead(int type, int *data);
int rotaryEncoderDeInit();

#endif
