# $Id: defs.pro 606 2004-12-25 03:08:40Z andrew $
#QMAKE_CXXFLAGS_DEBUG += -DLINUX
#QMAKE_CXXFLAGS += -DLINUX

win32 {
  QMAKE_CFLAGS_THREAD -= -mthreads
  QMAKE_LFLAGS_THREAD -= -mthreads
}

linux{
  QMAKE_CXXFLAGS += -DLINUX
  QMAKE_CXXFLAGS += -DLINUX
}


# detect prof version
rs_prof {
  exists(../qcadprof) {
	DEFINES += RS_PROF
    INCLUDEPATH += ../../qcadprof/include
  }
}


# detect fparser
!exists(../fparser) {
    DEFINES += RS_NO_FPARSER
}

# detect qcadcmd
!exists(../qcadcmd) {
    DEFINES += RS_NO_QCADCMD
}


# detect scripting support:
rs_scripting {
  exists(../qcadscripting) {
    DEFINES += RS_SCRIPTING
    INCLUDEPATH += ../../qcadscripting/include
  }
}

