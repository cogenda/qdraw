# $Id: qcadguiqt.pro 2390 2005-05-16 17:33:15Z andrew $
TEMPLATE    = lib
DESTDIR     = ../lib
CONFIG      += qt staticlib warn_on
INCLUDEPATH += ../include
INCLUDEPATH += ../../qcadlib/include ../../qcadactions/include ../../dxflib/include ../../fparser/include ../../qcadcmd/include

HEADERS     = \
              qg_actionfactory.h \
              qg_actionhandler.h \
              qg_blockwidget.h \
              qg_colorbox.h \
              qg_commandedit.h \
              qg_dialogfactory.h \
              qg_filedialog.h \
              qg_fontbox.h \
              qg_graphicview.h \
              qg_layerbox.h \
              qg_layerwidget.h \
              qg_linetypebox.h \
              qg_listviewitem.h \
              qg_mainwindowinterface.h \
              qg_patternbox.h \
              qg_pentoolbar.h \
              qg_qt2rs.h \
              qg_recentfiles.h \
              qg_scrollbar.h \
              qg_widthbox.h
SOURCES     = \
              qg_actionfactory.cpp \
              qg_actionhandler.cpp \
              qg_blockwidget.cpp \
              qg_colorbox.cpp \
              qg_commandedit.cpp \
              qg_dialogfactory.cpp \
              qg_filedialog.cpp \
              qg_fontbox.cpp \
              qg_graphicview.cpp \
              qg_layerbox.cpp \
              qg_layerwidget.cpp \
              qg_linetypebox.cpp \
              qg_listviewitem.cpp \
              qg_patternbox.cpp \
              qg_pentoolbar.cpp \
              qg_recentfiles.cpp \
              qg_widthbox.cpp
FORMS       = \
              ui/qg_commandwidget.ui \
              ui/qg_arcoptions.ui \
              ui/qg_arctangentialoptions.ui \
              ui/qg_beveloptions.ui \
              ui/qg_blockdialog.ui \
              ui/qg_cadtoolbar.ui \
              ui/qg_cadtoolbararcs.ui \
              ui/qg_cadtoolbarcircles.ui \
              ui/qg_cadtoolbardim.ui \
              ui/qg_cadtoolbarellipses.ui \
              ui/qg_cadtoolbarinfo.ui \
              ui/qg_cadtoolbarlines.ui \
              ui/qg_cadtoolbarmain.ui \
              ui/qg_cadtoolbarmodify.ui \
              ui/qg_cadtoolbarpoints.ui \
              ui/qg_cadtoolbarpolylines.ui \
              ui/qg_cadtoolbarselect.ui \
              ui/qg_cadtoolbarsnap.ui \
              ui/qg_cadtoolbarsplines.ui \
              ui/qg_circleoptions.ui \
              ui/qg_coordinatewidget.ui \
              ui/qg_dimensionlabeleditor.ui \
              ui/qg_dimlinearoptions.ui \
              ui/qg_dimoptions.ui \
              ui/qg_dlgattributes.ui \
              ui/qg_dlghatch.ui \
              ui/qg_dlginitial.ui \
              ui/qg_dlginsert.ui \
              ui/qg_dlgimageoptions.ui \
              ui/qg_dlgarc.ui \
              ui/qg_dlgcircle.ui \
              ui/qg_dlgdimension.ui \
              ui/qg_dlgdimlinear.ui \
              ui/qg_dlgline.ui \
              ui/qg_dlgellipse.ui \
              ui/qg_dlgmirror.ui \
              ui/qg_dlgmove.ui \
              ui/qg_dlgmoverotate.ui \
              ui/qg_dlgoptionsdrawing.ui \
              ui/qg_dlgoptionsgeneral.ui \
              ui/qg_dlgpoint.ui \
              ui/qg_dlgrotate.ui \
              ui/qg_dlgrotate2.ui \
              ui/qg_dlgscale.ui \
              ui/qg_dlgspline.ui \
              ui/qg_dlgtext.ui \
              ui/qg_exitdialog.ui \
              ui/qg_imageoptions.ui \
              ui/qg_insertoptions.ui \
              ui/qg_layerdialog.ui \
              ui/qg_libraryinsertoptions.ui \
              ui/qg_librarywidget.ui \
              ui/qg_lineangleoptions.ui \
              ui/qg_linebisectoroptions.ui \
              ui/qg_lineoptions.ui \
              ui/qg_lineparalleloptions.ui \
              ui/qg_lineparallelthroughoptions.ui \
              ui/qg_linepolygon2options.ui \
              ui/qg_linepolygonoptions.ui \
              ui/qg_linerelangleoptions.ui \
              ui/qg_mousewidget.ui \
              ui/qg_moverotateoptions.ui \
              ui/qg_printpreviewoptions.ui \
              ui/qg_roundoptions.ui \
              ui/qg_selectionwidget.ui \
              ui/qg_snapdistoptions.ui \
              ui/qg_splineoptions.ui \
              ui/qg_textoptions.ui \
              ui/qg_trimamountoptions.ui \
              ui/qg_widgetpen.ui
TRANSLATIONS = ts/qcadguiqt_cs.ts \
               ts/qcadguiqt_et.ts \
               ts/qcadguiqt_en.ts \
               ts/qcadguiqt_da.ts \
               ts/qcadguiqt_de.ts \
               ts/qcadguiqt_el.ts \
               ts/qcadguiqt_es.ts \
               ts/qcadguiqt_fr.ts \
               ts/qcadguiqt_hu.ts \
               ts/qcadguiqt_it.ts \
               ts/qcadguiqt_nl.ts \
               ts/qcadguiqt_no.ts \
               ts/qcadguiqt_pa.ts \
               ts/qcadguiqt_pl.ts \
               ts/qcadguiqt_ru.ts \
               ts/qcadguiqt_sk.ts \
               ts/qcadguiqt_tr.ts
TARGET      = qcadguiqt
OBJECTS_DIR = obj
MOC_DIR     = moc

exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
}
