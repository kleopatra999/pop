include Makefile.common

all:
	$(MAKE) -C pop $@
	$(MAKE) -C tests $@

clean:
	$(MAKE) -C pop $@
	$(MAKE) -C tests $@

.PHONY: all clean
