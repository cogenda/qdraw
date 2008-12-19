# Makefile for qcadlib

# qcadlib installation directories
#

# default: /usr/local/...
#LIBDIR = /usr/local/lib
#INCDIR = /usr/local/include/qcadlib

# users home dir:
LIBDIR = $(HOME)/local/lib
INCDIR = $(HOME)/local/include/qcadguiqt

FIND = find

all: prepare
	-rm -f ./lib/libqcadguiqt.a
	$(MAKE) ./lib/libqcadguiqt.a

prepare: src/Makefile
	if [ "x$$OS" = "xWindows_NT" ]; then \
	    sep="\\"; \
	else \
	    sep="/"; \
	fi; \
	echo "sep is $$sep"; \
	test -d ./include || mkdir -p ./include; \
	cd src; \
	$(MAKE) ui$${sep}qg_arcoptions.h; \
	$(MAKE) ui$${sep}qg_arctangentialoptions.h; \
	$(MAKE) ui$${sep}qg_beveloptions.h; \
	$(MAKE) ui$${sep}qg_blockdialog.h; \
	$(MAKE) ui$${sep}qg_cadtoolbar.h; \
	$(MAKE) ui$${sep}qg_cadtoolbararcs.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarcircles.h; \
	$(MAKE) ui$${sep}qg_cadtoolbardim.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarellipses.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarinfo.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarlines.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarmain.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarmodify.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarpoints.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarselect.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarsnap.h; \
	$(MAKE) ui$${sep}qg_cadtoolbarsplines.h; \
	$(MAKE) ui$${sep}qg_circleoptions.h; \
	$(MAKE) ui$${sep}qg_commandwidget.h; \
	$(MAKE) ui$${sep}qg_coordinatewidget.h; \
	$(MAKE) ui$${sep}qg_dimlinearoptions.h; \
	$(MAKE) ui$${sep}qg_dimensionlabeleditor.h; \
	$(MAKE) ui$${sep}qg_dimoptions.h; \
	$(MAKE) ui$${sep}qg_dlgarc.h; \
	$(MAKE) ui$${sep}qg_dlgattributes.h; \
	$(MAKE) ui$${sep}qg_dlgcircle.h; \
	$(MAKE) ui$${sep}qg_dlgdimension.h; \
	$(MAKE) ui$${sep}qg_dlgdimlinear.h; \
	$(MAKE) ui$${sep}qg_dlghatch.h; \
	$(MAKE) ui$${sep}qg_dlgimageoptions.h; \
	$(MAKE) ui$${sep}qg_dlginitial.h; \
	$(MAKE) ui$${sep}qg_dlginsert.h; \
	$(MAKE) ui$${sep}qg_dlgline.h; \
	$(MAKE) ui$${sep}qg_dlgellipse.h; \
	$(MAKE) ui$${sep}qg_dlgmirror.h; \
	$(MAKE) ui$${sep}qg_dlgmove.h; \
	$(MAKE) ui$${sep}qg_dlgmoverotate.h; \
	$(MAKE) ui$${sep}qg_dlgoptionsdrawing.h; \
	$(MAKE) ui$${sep}qg_dlgoptionsgeneral.h; \
	$(MAKE) ui$${sep}qg_dlgpoint.h; \
	$(MAKE) ui$${sep}qg_dlgrotate.h; \
	$(MAKE) ui$${sep}qg_dlgrotate2.h; \
	$(MAKE) ui$${sep}qg_dlgscale.h; \
	$(MAKE) ui$${sep}qg_dlgtext.h; \
	$(MAKE) ui$${sep}qg_exitdialog.h; \
	$(MAKE) ui$${sep}qg_imageoptions.h; \
	$(MAKE) ui$${sep}qg_insertoptions.h; \
	$(MAKE) ui$${sep}qg_layerdialog.h; \
	$(MAKE) ui$${sep}qg_librarywidget.h; \
	$(MAKE) ui$${sep}qg_lineangleoptions.h; \
	$(MAKE) ui$${sep}qg_linebisectoroptions.h; \
	$(MAKE) ui$${sep}qg_lineoptions.h; \
	$(MAKE) ui$${sep}qg_lineparalleloptions.h; \
	$(MAKE) ui$${sep}qg_lineparallelthroughoptions.h; \
	$(MAKE) ui$${sep}qg_linepolygon2options.h; \
	$(MAKE) ui$${sep}qg_linepolygonoptions.h; \
	$(MAKE) ui$${sep}qg_linerelangleoptions.h; \
	$(MAKE) ui$${sep}qg_mousewidget.h; \
	$(MAKE) ui$${sep}qg_moverotateoptions.h; \
	$(MAKE) ui$${sep}qg_printpreviewoptions.h; \
	$(MAKE) ui$${sep}qg_roundoptions.h; \
	$(MAKE) ui$${sep}qg_selectionwidget.h; \
	$(MAKE) ui$${sep}qg_snapdistoptions.h; \
	$(MAKE) ui$${sep}qg_textoptions.h; \
	$(MAKE) ui$${sep}qg_trimamountoptions.h; \
	$(MAKE) ui$${sep}qg_widgetpen.h; \
	cd ../include; rm -f *.h; \
	for hf in `find ../src -name '*.h'`; do \
		if [ "x$$OS" = "xWindows_NT" ]; then \
			cp "$$hf" .; \
		else \
			ln -s "$$hf" 2> /dev/null; \
		fi \
	done
	
./lib/libqcadguiqt.a: src/Makefile
	cd src && $(MAKE)

src/Makefile:
	cd src && qmake qcadguiqt.pro

install: ./lib/libqcadguiqt.a
	test -d $(LIBDIR) || mkdir -p $(LIBDIR)
	test -d $(INCDIR) || mkdir -p $(INCDIR)
	cp ./lib/libqcadguiqt.a $(LIBDIR)
	cp ./include/*.h $(INCDIR)

clean:
	cd src && $(MAKE) clean
	cd test && $(MAKE) clean
	-rm -f ./lib/lib*
	-$(FIND) . -name "*~" -exec rm -f {} \;
  
testing: ./lib/libqcadguiqt.a
	-(cd ./test ; make clean; make)

docu:
	-(doxygen ./doxygen.cfg)

statistics:
	@echo "Lines in source files:"
	-find -name *.cpp | xargs cat | wc
	@echo "Lines in header files:"
	-find -name *.h | xargs cat | wc

dist:	clean

