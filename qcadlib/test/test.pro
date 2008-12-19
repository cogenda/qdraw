# $Id: qcad.pro 935 2005-02-28 18:15:07Z andrew $
TEMPLATE = app
CONFIG += qt warn_on link_prl

# global defs
exists( ../../../mkspecs/defs.pro ) {
  include( ../../../mkspecs/defs.pro )
}

# Application specific defines:

rs_scripting {
    LIBS += -L../../qcadscripting/lib -lqcadscripting
}
LIBS += \
        -L../../qcadguiqt/lib -lqcadguiqt \
        -L../../qcadactions/lib -lqcadactions \
        -L../../qcadcmd/lib -lqcadcmd
rs_prof {
    LIBS +=  -L../../qcadprof/lib -lqcadprof
}
LIBS += \
        -L../../qcadlib/lib -lqcad \
        -L../../qcadcmd/lib -lqcadcmd \
        -L../../dxflib/lib -ldxf \
        -L../../fparser/lib -lfparser
rs_cam {
    LIBS += -L../../qcadcam/lib -lqcadcam
}


# include paths:
INCLUDEPATH += ../../fparser/include \
               ../../dxflib/include \
               ../../qcadlib/include \
               ../../qcadcmd/include \
               ../../qcadactions/include \
               ../../qcadguiqt/include


SOURCES        = test.cpp

OBJECTS_DIR = obj
MOC_DIR     = moc


