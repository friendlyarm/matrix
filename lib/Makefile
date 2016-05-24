CROSS_COMPILE ?=

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib

CFLAGS	= -Wall -Os
LDFLAGS	= -lpthread
LIB		= libfahw.so
SLIB	= libfahw.a
CFLAGS	+= -fPIC

OBJDIR	= .obj
DEPDIR	= .deps
LIBSRC	= $(wildcard *.c)
LIBOBJ	= $(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(LIBSRC))))
DEPS	= $(patsubst %.c,$(DEPDIR)/%.Po,$(LIBSRC))

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
	install ./includes/libfahw*.h /usr/local/include
	install --mode=644 libfahw.so /usr/local/lib
	install --mode=644 libfahw.a /usr/local/lib
	ldconfig
clean:
	rm -rf $(LIB) $(LIBOBJ) $(SLIB) $(DEPS) $(OBJDIR) $(DEPDIR)
