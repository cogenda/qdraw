# $Id: defs.pro 606 2004-12-25 03:08:40Z andrew $
QMAKE_CXXFLAGS_DEBUG += -pedantic
QMAKE_CXXFLAGS += -pedantic

win32 {
  QMAKE_CFLAGS_THREAD -= -mthreads
  QMAKE_LFLAGS_THREAD -= -mthreads
}

# detect prof version
rs_prof {
  exists(../qcadprof) {
	DEFINES += RS_PROF
    INCLUDEPATH += ../../qcadprof/include
  }
}

# detect demo version
rs_demo {
	DEFINES += RS_DEMO
}

# detect fparser
!exists(../fparser) {
    DEFINES += RS_NO_FPARSER
}

# detect qcadcmd
!exists(../qcadcmd) {
    DEFINES += RS_NO_QCADCMD
}

# detect cam support
rs_cam {
  exists(../qcadcam) {
    DEFINES += RS_CAM
    INCLUDEPATH += ../../qcadcam/include
  }
}

# detect scripting support:
rs_scripting {
  exists(../qcadscripting) {
    DEFINES += RS_SCRIPTING
    INCLUDEPATH += ../../qcadscripting/include
  }
}

