SUBDIRS				= $(wildcard matrix*)
TARGET				= subdirs
PHONY			   += $(TARGET) $(SUBDIRS) %.clean

all: $(TARGET)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

%.clean:
	@(cd $(patsubst %.clean, %, $@) && $(MAKE) clean)

clean distclean: $(patsubst %, %.clean, $(SUBDIRS))

install: 
	mkdir -p $(INSTALLDIR)/bin
	for app in $(wildcard matrix*); do \
	  install --mode=755 $$app/$$app $(INSTALLDIR)/bin; \
	done;  
	
.PHONY: $(PHONY) install clean distclean