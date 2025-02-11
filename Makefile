ifeq ($(PLATFORM),)
	PLATFORM := SDL2
endif

.PHONY: clean

all:
	make -f Makefile.$(PLATFORM)

clean:
	make -f Makefile.$(PLATFORM) clean
