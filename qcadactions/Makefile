# Makefile for qcadactions

VERSION = 1.9.5

# qcadactions installation directories
#

# default: /usr/local/...
#LIBDIR = /usr/local/lib
#INCDIR = /usr/local/include/qcadactions

# users home dir:
LIBDIR = $(HOME)/local/lib
INCDIR = $(HOME)/local/include/qcadactions

FIND = find

all:
	$(MAKE) prepare
	-rm -f ./lib/libqcadactions.a
	$(MAKE) ./lib/libqcadactions.a

prepare:
	test -d ./include || mkdir -p ./include
	-( cd ./include; rm -f *.h; \
	for hf in `find ../src -name '*.h'`; do \
		if [ "x$$OS" = "xWindows_NT" ]; then \
			cp "$$hf" .; \
		else \
			ln -s "$$hf" 2> /dev/null; \
		fi \
	done )

./lib/libqcadactions.a: src/Makefile
	$(MAKE) src/Makefile
	cd src && $(MAKE)

src/Makefile:
	cd src && qmake qcadactions.pro

install: ./lib/libqcadactions.a
	test -d $(LIBDIR) || mkdir -p $(LIBDIR)
	test -d $(INCDIR) || mkdir -p $(INCDIR)
	cp ./lib/libqcadactions.a $(LIBDIR)
	cp ./include/*.h $(INCDIR)

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean
	-rm -f ./lib/lib*
	-$(FIND) . -name "*~" -exec rm -f {} \;
  
testing: ./lib/libqcadactions.a
	-(cd ./test ; make clean; make)

docu:
	-(doxygen ./doxygen.cfg)

statistics:
	@echo "Lines in source files:"
	-find -name *.cpp | xargs cat | wc
	@echo "Lines in header files:"
	-find -name *.h | xargs cat | wc

indent:
	-find . -name "*.cpp" | xargs astyle --style=kr
	-find . -name "*.h" | xargs astyle --style=kr
	-find . -name "*.h.orig" | xargs rm
	-find . -name "*.cpp.orig" | xargs rm

dist:	clean
	-rm -d ../doc/classref/html/en/*
	-(cd .. ; cp -r ./qcadactions ./qcadactions_dist)
	-(cd ../qcadactions_dist/qcadactions ; rm config.log config.cache reinstall*)
	-(cd ../qcadactions_dist ; tar fvcz qcadactions-$(VERSION).tar.gz qcadactions)

