# ----------------------------------------------------------------------------
# Makefile for building fa-hardware
#
# Copyright 2015 FriendlyARM (http://www.arm9.net/)
#

# Usage example:
# make CROSS_COMPILE=arm-linux-gnueabihf-
# make CROSS_COMPILE=arm-linux-gnueabihf- clean

CROSS_COMPILE =
ifeq ($(CROSS_COMPILE),)
$(error "Not set CROSS_COMPILE=";)
endif

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib

export AS LD CC AR NM STRIP OBJCOPY OBJDUMP RANLIB CROSS_COMPILE

SUBDIRS				= lib \
				  demo 

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

