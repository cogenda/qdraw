# $Id: qcadactions.pro 2389 2005-05-16 17:33:05Z andrew $
TEMPLATE    = lib
DESTDIR     = ../lib
CONFIG      += qt staticlib warn_on
INCLUDEPATH += ../include
INCLUDEPATH += ../../dxflib/include ../../qcadlib/include ../../fparser/include ../../qcadcmd/include

HEADERS     = rs_actionblocksadd.h \
              rs_actionblocksattributes.h \
              rs_actionblockscreate.h \
              rs_actionblocksedit.h \
              rs_actionblocksexplode.h \
              rs_actionblocksinsert.h \
              rs_actionblocksfreezeall.h \
              rs_actionblocksremove.h \
              rs_actionblockstoggleview.h \
              rs_actiondefault.h \
              rs_actiondimaligned.h \
              rs_actiondimangular.h \
              rs_actiondimdiametric.h \
              rs_actiondimension.h \
              rs_actiondimleader.h \
              rs_actiondimlinear.h \
              rs_actiondimradial.h \
              rs_actiondrawarc.h \
              rs_actiondrawarc3p.h \
              rs_actiondrawarctangential.h \
              rs_actiondrawcircle.h \
              rs_actiondrawcircle2p.h \
              rs_actiondrawcircle3p.h \
              rs_actiondrawcirclecr.h \
              rs_actiondrawellipseaxis.h \
              rs_actiondrawhatch.h \
              rs_actiondrawimage.h \
              rs_actiondrawline.h \
              rs_actiondrawlineangle.h \
              rs_actiondrawlinebisector.h \
              rs_actiondrawlinefree.h \
              rs_actiondrawlinehorvert.h \
              rs_actiondrawlineparallel.h \
              rs_actiondrawlineparallelthrough.h \
              rs_actiondrawlinepolygon.h \
              rs_actiondrawlinepolygon2.h \
              rs_actiondrawlinerectangle.h \
              rs_actiondrawlinerelangle.h \
              rs_actiondrawlinetangent1.h \
              rs_actiondrawlinetangent2.h \
              rs_actiondrawpoint.h \
              rs_actiondrawspline.h \
              rs_actiondrawtext.h \
              rs_actioneditcopy.h \
              rs_actioneditpaste.h \
              rs_actioneditundo.h \
              rs_actionfilenew.h \
              rs_actionfileopen.h \
              rs_actionfilesave.h \
              rs_actionfilesaveas.h \
              rs_actioninfoangle.h \
              rs_actioninfoarea.h \
              rs_actioninfoinside.h \
              rs_actioninfodist.h \
              rs_actioninfodist2.h \
              rs_actioninfototallength.h \
              rs_actionlayersadd.h \
              rs_actionlayersedit.h \
              rs_actionlayersfreezeall.h \
              rs_actionlayersremove.h \
              rs_actionlayerstogglelock.h \
              rs_actionlayerstoggleview.h \
              rs_actionlibraryinsert.h \
              rs_actionlockrelativezero.h \
              rs_actionmodifyattributes.h \
              rs_actionmodifybevel.h \
              rs_actionmodifycut.h \
              rs_actionmodifydelete.h \
              rs_actionmodifydeletefree.h \
              rs_actionmodifydeletequick.h \
              rs_actionmodifyentity.h \
              rs_actionmodifymirror.h \
              rs_actionmodifymove.h \
              rs_actionmodifymoverotate.h \
              rs_actionmodifyrotate.h \
              rs_actionmodifyrotate2.h \
              rs_actionmodifyround.h \
              rs_actionmodifyscale.h \
              rs_actionmodifystretch.h \
              rs_actionmodifytrim.h \
              rs_actionmodifytrimamount.h \
              rs_actionmodifyexplodetext.h \
              rs_actionoptionsdrawing.h \
              rs_actionparisdebugcreatecontainer.h \
              rs_actionprintpreview.h \
              rs_actionselect.h \
              rs_actionselectall.h \
              rs_actionselectbase.h \
              rs_actionselectcontour.h \
              rs_actionselectintersected.h \
              rs_actionselectinvert.h \
              rs_actionselectsingle.h \
              rs_actionselectwindow.h \
              rs_actionselectlayer.h \
              rs_actionsetrelativezero.h \
              rs_actionsetsnapmode.h \
              rs_actionsetsnaprestriction.h \
              rs_actionsnapintersectionmanual.h \
              rs_actiontoolregeneratedimensions.h \
              rs_actionzoomauto.h \
              rs_actionzoomautoy.h \
              rs_actionzoomin.h \
              rs_actionzoompan.h \
              rs_actionzoomprevious.h \
              rs_actionzoomredraw.h \
              rs_actionzoomscroll.h \
              rs_actionzoomwindow.h

SOURCES     = rs_actionblocksadd.cpp \
              rs_actionblocksattributes.cpp \
              rs_actionblockscreate.cpp \
              rs_actionblocksedit.cpp \
              rs_actionblocksexplode.cpp \
              rs_actionblocksinsert.cpp \
              rs_actionblocksfreezeall.cpp \
              rs_actionblocksremove.cpp \
              rs_actionblockstoggleview.cpp \
              rs_actiondefault.cpp \
              rs_actiondimaligned.cpp \
              rs_actiondimangular.cpp \
              rs_actiondimdiametric.cpp \
              rs_actiondimension.cpp \
              rs_actiondimleader.cpp \
              rs_actiondimlinear.cpp \
              rs_actiondimradial.cpp \
              rs_actiondrawarc.cpp \
              rs_actiondrawarc3p.cpp \
              rs_actiondrawarctangential.cpp \
              rs_actiondrawcircle.cpp \
              rs_actiondrawcircle2p.cpp \
              rs_actiondrawcircle3p.cpp \
              rs_actiondrawcirclecr.cpp \
              rs_actiondrawellipseaxis.cpp \
              rs_actiondrawhatch.cpp \
              rs_actiondrawimage.cpp \
              rs_actiondrawline.cpp \
              rs_actiondrawlineangle.cpp \
              rs_actiondrawlinebisector.cpp \
              rs_actiondrawlinefree.cpp \
              rs_actiondrawlinehorvert.cpp \
              rs_actiondrawlineparallel.cpp \
              rs_actiondrawlineparallelthrough.cpp \
              rs_actiondrawlinepolygon.cpp \
              rs_actiondrawlinepolygon2.cpp \
              rs_actiondrawlinerectangle.cpp \
              rs_actiondrawlinerelangle.cpp \
              rs_actiondrawlinetangent1.cpp \
              rs_actiondrawlinetangent2.cpp \
              rs_actiondrawpoint.cpp \
              rs_actiondrawspline.cpp \
              rs_actiondrawtext.cpp \
              rs_actioneditcopy.cpp \
              rs_actioneditpaste.cpp \
              rs_actioneditundo.cpp \
              rs_actionfilenew.cpp \
              rs_actionfileopen.cpp \
              rs_actionfilesave.cpp \
              rs_actionfilesaveas.cpp \
              rs_actioninfoangle.cpp \
              rs_actioninfoarea.cpp \
              rs_actioninfoinside.cpp \
              rs_actioninfodist.cpp \
              rs_actioninfodist2.cpp \
              rs_actioninfototallength.cpp \
              rs_actionlayersadd.cpp \
              rs_actionlayersedit.cpp \
              rs_actionlayersfreezeall.cpp \
              rs_actionlayersremove.cpp \
              rs_actionlayerstogglelock.cpp \
              rs_actionlayerstoggleview.cpp \
              rs_actionlibraryinsert.cpp \
              rs_actionlockrelativezero.cpp \
              rs_actionmodifyattributes.cpp \
              rs_actionmodifybevel.cpp \
              rs_actionmodifycut.cpp \
              rs_actionmodifydelete.cpp \
              rs_actionmodifydeletefree.cpp \
              rs_actionmodifydeletequick.cpp \
              rs_actionmodifyentity.cpp \
              rs_actionmodifymirror.cpp \
              rs_actionmodifymove.cpp \
              rs_actionmodifymoverotate.cpp \
              rs_actionmodifyrotate.cpp \
              rs_actionmodifyrotate2.cpp \
              rs_actionmodifyround.cpp \
              rs_actionmodifyscale.cpp \
              rs_actionmodifystretch.cpp \
              rs_actionmodifytrim.cpp \
              rs_actionmodifytrimamount.cpp \
              rs_actionmodifyexplodetext.cpp \
              rs_actionoptionsdrawing.cpp \
              rs_actionparisdebugcreatecontainer.cpp \
              rs_actionprintpreview.cpp \
              rs_actionselect.cpp \
              rs_actionselectall.cpp \
              rs_actionselectbase.cpp \
              rs_actionselectcontour.cpp \
              rs_actionselectintersected.cpp \
              rs_actionselectinvert.cpp \
              rs_actionselectsingle.cpp \
              rs_actionselectwindow.cpp \
              rs_actionselectlayer.cpp \
              rs_actionsetrelativezero.cpp \
              rs_actionsetsnapmode.cpp \
              rs_actionsetsnaprestriction.cpp \
              rs_actionsnapintersectionmanual.cpp \
              rs_actiontoolregeneratedimensions.cpp \
              rs_actionzoomauto.cpp \
              rs_actionzoomautoy.cpp \
              rs_actionzoomin.cpp \
              rs_actionzoompan.cpp \
              rs_actionzoomprevious.cpp \
              rs_actionzoomredraw.cpp \
              rs_actionzoomscroll.cpp \
              rs_actionzoomwindow.cpp

TRANSLATIONS = ts/qcadactions_cs.ts \
               ts/qcadactions_en.ts \
               ts/qcadactions_et.ts \
               ts/qcadactions_da.ts \
               ts/qcadactions_de.ts \
               ts/qcadactions_el.ts \
               ts/qcadactions_es.ts \
               ts/qcadactions_fr.ts \
               ts/qcadactions_hu.ts \
               ts/qcadactions_it.ts \
               ts/qcadactions_nl.ts \
               ts/qcadactions_no.ts \
               ts/qcadactions_pa.ts \
               ts/qcadactions_pl.ts \
               ts/qcadactions_ru.ts \
               ts/qcadactions_sk.ts \
               ts/qcadactions_tr.ts

IMAGES = \
		images/configure.png \
		images/editcopy2.png \
		images/editcut2.png \
		images/editpaste2.png \
		images/exit.png \
		images/fileclose.png \
		images/filenew.png \
		images/fileopen2.png \
		images/fileprint.png \
		images/fileprintpreview.png \
		images/filesave2.png \
		images/filesaveas.png \
		images/redo2.png \
		images/undo2.png \
		images/viewgrid.png \
		images/viewdraft.png \
		images/zoomauto.png \
		images/zoomin.png \
		images/zoomout.png \
		images/zoompan.png \
		images/zoomprevious.png \
		images/zoomredraw.png \
		images/zoomwindow.png

TARGET      = qcadactions
OBJECTS_DIR = obj
MOC_DIR     = moc

exists( ../../mkspecs/defs.pro ) {
    include( ../../mkspecs/defs.pro )
}

