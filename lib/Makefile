CROSS_COMPILE ?=
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

INSTALLPATH=/tmp/libfahw
CFLAGS=-Wall -Os
LDFLAGS=-lpthread
LIB=libfahw.so
SLIB=libfahw.a
CFLAGS+=-fPIC

OBJDIR=.obj
DEPDIR=.deps
LIBSRC=common.c gpio.c i2c.c  w1.c led.c pwm.c iio.c filectl.c pcf8591.c
LIBSRC+=pcf8574.c adxl34x.c gpio_sensor.c hmc5883.c pwm.c mcp23017.c
LIBOBJ=$(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(LIBSRC))))
DEPS=$(patsubst %.c,$(DEPDIR)/%.Po,$(LIBSRC))


MV=mv

all: $(LIB) $(SLIB)
lib: $(LIB)
slib: $(SLIB)
$(OBJDIR)/%.o: %.c
	@test -d $(DEPDIR) || mkdir -p $(DEPDIR)
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $< -I ./includes
	@mv $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
	
-include $(DEPS)
$(LIB): $(LIBOBJ)
	$(CC) $(LDFLAGS) -shared -o $@ $^
$(SLIB): $(LIBOBJ)
	$(AR) rcu $@ $^
	$(RANLIB) $@
install: $(LIB)
	rm -rf $(INSTALLPATH)
	mkdir -p $(INSTALLPATH)/includes
	mkdir -p $(INSTALLPATH)/libs
	cp libfahw*.h $(INSTALLPATH)/includes/
	cp libfahw.so $(INSTALLPATH)/libs/
	cp libfahw.a $(INSTALLPATH)/libs/
clean:
	rm -rf $(LIB) $(LIBOBJ) $(SLIB) $(DEPS) $(OBJDIR) $(DEPDIR)
