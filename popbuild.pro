TEMPLATE = subdirs
SUBDIRS = \
	pop \
	tests \
	tools/popast.pro \
	tools/popc.pro \
	tools/popcc.pro \
	tools/popdis.pro \
	tools/poptok.pro \
	tools/popvm.pro

tools.depends = pop
tests.depends = pop
