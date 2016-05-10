SUBDIRS			= $(wildcard matrix*)
TARGET			= subdirs
PHONY			+= $(TARGET) $(SUBDIRS) %.clean

all: $(TARGET)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

%.clean:
	@(cd $(patsubst %.clean, %, $@) && $(MAKE) clean)

clean distclean: $(patsubst %, %.clean, $(SUBDIRS))

install: 
	mkdir -p $(INSTALLDIR)/usr/bin
	for demo_folder in $(wildcard matrix*); do \
		cd $$demo_folder; \
		install --mode=755 matrix* /usr/bin; \
		cd ..;\
	done;  
	
.PHONY: $(PHONY) install clean distclean
