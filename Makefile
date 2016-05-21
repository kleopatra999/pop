include Makefile.common

all:
	$(MAKE) -C pop $@
	$(MAKE) -C tools $@
	$(MAKE) -C tests $@

clean:
	$(MAKE) -C pop $@
	$(MAKE) -C tools $@
	$(MAKE) -C tests $@

.PHONY: all clean
