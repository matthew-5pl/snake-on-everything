ifeq ($(PLATFORM),)
	PLATFORM := SDL2
endif

MAKEFILES := $(wildcard Makefile.*)

.PHONY: clean cleanall

all:
	make -f Makefile.$(PLATFORM)

clean:
	make -f Makefile.$(PLATFORM) clean

cleanall:
	@for mk in $(MAKEFILES); do \
		echo "Running make clean for $$mk"; \
		make -f $$mk clean 1> /dev/null 2> /dev/null; \
	done