# $Id: qcadcmd.pro 2388 2005-05-16 17:32:52Z andrew $
TEMPLATE    = lib
DESTDIR     = ../lib
CONFIG      += qt staticlib warn_on
INCLUDEPATH += ../include
INCLUDEPATH += ../../qcadlib/include ../../fparser/include

HEADERS     = rs_commands.h
SOURCES     = rs_commands.cpp 
TRANSLATIONS = ts/qcadcmd_cs.ts \
               ts/qcadcmd_et.ts \
               ts/qcadcmd_en.ts \
               ts/qcadcmd_da.ts \
               ts/qcadcmd_de.ts \
               ts/qcadcmd_el.ts \
               ts/qcadcmd_es.ts \
               ts/qcadcmd_fr.ts \
               ts/qcadcmd_hu.ts \
               ts/qcadcmd_it.ts \
               ts/qcadcmd_nl.ts \
               ts/qcadcmd_no.ts \
               ts/qcadcmd_pa.ts \
               ts/qcadcmd_pl.ts \
               ts/qcadcmd_ru.ts \
               ts/qcadcmd_sk.ts \
               ts/qcadcmd_tr.ts
TARGET      = qcadcmd
OBJECTS_DIR = obj
MOC_DIR     = moc


exists( ../../mkspecs/defs.pro ) {
	include( ../../mkspecs/defs.pro )
}
