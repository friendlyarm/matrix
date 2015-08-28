# ----------------------------------------------------------------------------
# Makefile for building demo
#
# Copyright 2015 FriendlyARM (http://www.arm9.net/)
#

SUBDIRS				= \
matrix-3_axis_digital_accelerometer \
matrix-analog_to_digital_converter \
matrix-button \
matrix-buzzer \
matrix-compass \
matrix-i2c_lcd1602 \
matrix-i2c_lcd1602_key \
matrix-joystick \
matrix-led \
matrix-relay \
matrix-sound_sensor \
matrix-temperature_and_humidity_sensor \
matrix-temperature_sensor \
matrix-ultrasonic_ranger \

TARGET				= subdirs
PHONY			   += $(TARGET) $(SUBDIRS) %.clean

all: $(TARGET)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

%.clean:
	@(cd $(patsubst %.clean, %, $@) && $(MAKE) clean)

clean distclean: $(patsubst %, %.clean, $(SUBDIRS))

# ----------------------------------------------------------------------------

.PHONY: $(PHONY) install clean distclean

# End of file
# vim: syntax=make

