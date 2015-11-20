#ifndef __FRIENDLYARM_HARDWARE_W1_H__
#define __FRIENDLYARM_HARDWARE_W1_H__

#define DS18B20_SYS_PATH                "/sys/devices/w1_bus_master1/"
int ds18b20Read(char * temperature);

#endif
