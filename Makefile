include Makefile.common

all:
	$(MAKE) -C pop $@

clean:
	$(MAKE) -C pop $@

.PHONY: all clean
