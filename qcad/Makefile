# Makefile for QCad

# QCad installation directories
#

# default: /usr/local/...
#LIBDIR = /usr/local/lib
#INCDIR = /usr/local/include/qcadlib

# users home dir:
INSTALLDIR = $(HOME)/local

FIND = find

all: prepare qcad

prepare:

qcad: src/Makefile src/*.h src/*.cpp ../dxflib/lib/libdxf.a ../qcadguiqt/lib/libqcadguiqt.a ../qcadlib/lib/libqcad.a
	cd src && $(MAKE)

src/Makefile: src/qcad.pro
	cd src && qmake qcad.pro

clean:
	cd src && $(MAKE) clean
	- rm qcad
	- rm camexpert
	-$(FIND) . -name "*~" -exec rm -f {} \;
  
docu:
	-(doxygen ./doxygen.cfg)

statistics:
	@echo "Lines in source files:"
	-find -name *.cpp | xargs cat | wc
	@echo "Lines in header files:"
	-find -name *.h | xargs cat | wc

dist:	clean

