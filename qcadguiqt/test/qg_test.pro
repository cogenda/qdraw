TEMPLATE	= app
CONFIG		+= qt warn_on debug

unix {
	LIBS += -L../lib -lqcadguiqt \
	        -L../../qcadactions/lib -lqcadactions \
	        -L../../qcadlib/lib -lqcad \
			-L../../dxflib/lib -ldxf
}

win32 {
	LIBS += ../lib/qcadguiqt.lib \
	        ../../qcadactions/lib/qcadactions.lib \
	        ../../qcadlib/lib/qcad.lib \
		    ../../dxflib/lib/dxflib.lib
}

INCLUDEPATH += ../../dxflib/include \
               ../../qcadlib/include \
               ../../qcadactions/include \
			   ../include

# detect Qt:
exists( $$(QTDIR)/lib/libqt-mt.so ) {
    CONFIG += thread
}

HEADERS		= qg_test.h
SOURCES		= qg_test.cpp \
              main.cpp 
TARGET		= qg_test
OBJECTS_DIR = obj
MOC_DIR     = moc
