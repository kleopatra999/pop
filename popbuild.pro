TEMPLATE = subdirs
SUBDIRS = \
	pop/libpop.pro \
	pop/pop.pro \
	tests

pop/pop.pro.depends = pop/libpop.pro
tests.depends = pop/libpop.pro
