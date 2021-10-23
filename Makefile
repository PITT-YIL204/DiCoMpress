#transfer to makefile in src directory

default: all

.DEFAULT:
	cd src && $(MAKE) $@