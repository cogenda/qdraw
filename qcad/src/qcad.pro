# $Id: qcad.pro 2391 2005-05-16 17:33:20Z andrew $
TEMPLATE = app
CONFIG += qt warn_on link_prl

exists( custom.pro ) {
	include( custom.pro )
}

# global defs
exists( ../../mkspecs/defs.pro ) {
  include( ../../mkspecs/defs.pro )
}

# Application specific targets:

# CAM Expert:
rs_cam {
  rs_demo {
    rs_apple {
      TARGET = CAMExpertDemo
    }
    else {
      TARGET = camexpert_demo
    }
  }
  else {
    rs_apple {
      TARGET = CAMExpert
    }
    else {
      TARGET = camexpert
    }
  }
}

# QCad:
else {
  rs_demo {
    rs_apple {
      TARGET = QCadDemo
    }
    else {
      TARGET = qcad_demo
    }
  }
  else {
    rs_apple {
      TARGET = QCad
    }
    else {
      TARGET = qcad
    }
  }
}


# Application specific defines:

# CAM Expert:
rs_cam {
	rs_demo {
    	DEFINES += QC_APPKEY="\"/CAMExpertDemo\"" QC_APPDIR="\"camexpertdemo\""
		DEFINES += QC_APPNAME="\"CAM Expert Demo\""
	}
	else {
    	DEFINES += QC_APPKEY="\"/CAMExpert\"" QC_APPDIR="\"camexpert\""
		DEFINES += QC_APPNAME="\"CAM Expert\""
	}
}

# QCad:
else {
	rs_custom {
		#allows override app name with custom one
	}
	else {
		rs_demo {
			DEFINES += QC_APPKEY="\"/QCadDemo\"" QC_APPDIR="\"qcaddemo\""
			DEFINES += QC_APPNAME="\"QCad Demo\""
		}
		else {
			DEFINES += QC_APPKEY="\"/QCad\"" QC_APPDIR="\"qcad\""
			DEFINES += QC_APPNAME="\"QCad\""
		}
	}
}


# libraries:
LIBS += -lqassistantclient

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
        -L../../dxflib/lib -ldxf \
        -L../../fparser/lib -lfparser
rs_cam {
    LIBS += -L../../qcadcam/lib -lqcadcam
}


# icon
!rs_custom {
	win32:RC_FILE = qcad.rc
	unix:RC_FILE = qcad.icns
}

# include paths:
INCLUDEPATH += ../../fparser/include \
               ../../dxflib/include \
               ../../qcadlib/include \
               ../../qcadcmd/include \
               ../../qcadactions/include \
               ../../qcadguiqt/include


HEADERS        = qc_applicationwindow.h \
                 qc_dialogfactory.h \
                 qc_graphicview.h \
                 qc_mdiwindow.h \
                 main.h
SOURCES        = qc_applicationwindow.cpp \
                 qc_dialogfactory.cpp \
                 qc_graphicview.cpp \
                 qc_mdiwindow.cpp \
                 main.cpp 
TRANSLATIONS = ts/qcad_cs.ts \
               ts/qcad_et.ts \
               ts/qcad_en.ts \
               ts/qcad_da.ts \
               ts/qcad_de.ts \
               ts/qcad_el.ts \
               ts/qcad_es.ts \
               ts/qcad_fr.ts \
               ts/qcad_hu.ts \
               ts/qcad_it.ts \
               ts/qcad_nl.ts \
               ts/qcad_no.ts \
               ts/qcad_pa.ts \
               ts/qcad_pl.ts \
               ts/qcad_ru.ts \
               ts/qcad_sk.ts \
               ts/qcad_tr.ts

IMAGES += \
         images/contents.png \
         images/document.png \
         images/editclear.png \
         images/qcad16.png \
         images/qcad.png

DESTDIR     = ..

OBJECTS_DIR = obj
MOC_DIR     = moc

# load scripting support:
rs_scripting {
  #load( qsa )
  LIBS += -lqsa
}

exists( custom.cpp ) {
  SOURCES += custom.cpp
}

