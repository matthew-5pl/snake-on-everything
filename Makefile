ifeq ($(PLATFORM),)
	PLATFORM := SDL2
endif

all:
	make -f Makefile.$(PLATFORM)

clean:
	make -f Makefile.$(PLATFORM) clean