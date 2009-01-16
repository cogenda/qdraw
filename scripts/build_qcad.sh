#!/bin/bash
# $Id: build_qcad.sh 2488 2005-07-20 08:33:25Z andrew $

echo "build_qcad.sh"
echo "Usage: ./build_qcad.sh [options]"
echo "options:"
echo "  noclean     don't clean (speeds up building if the options don't change)"
echo "  noconfig    don't run configure (speeds up building if the options don't change)"
echo "  noprepare   don't run prepare (speeds up building if the options don't change)"
echo "  notrans     don't generate translations"
echo "  distcc      use distcc for distributed compilation. DISTCC_HOSTS must be set."
echo

echo "QTDIR is: $QTDIR"
echo "QMAKESPEC is: $QMAKESPEC"

# lib file extersion
libext=a

# detect system:
if [ "x$OS" == "xWindows_NT" ]
then
# cygin make
    export MAKE=make
# msvc nmake    
    export NMAKE=nmake
    echo "Platform is Windows"
    platform=win32
    QMAKE_OPT="$QMAKE_OPT CONFIG+=win32"
    libext=lib
    targetpostfix=.exe
elif [ `uname` == "SunOS" ]
then
    export MAKE=gmake
    echo "Platform is Solaris"
    platform=solaris
    QMAKE_OPT="$QMAKE_OPT CONFIG+=solaris"
elif [ "x$OSTYPE" == "xdarwin8.0" ]
then
    export MAKE=make
    echo "Platform is Mac OS X"
    platform=osx
    QMAKE_OPT="$QMAKE_OPT CONFIG+=rs_apple"
elif [ `uname` == "FreeBSD" ]
then
    export MAKE=gmake
    echo "Platform is FreeBSD"
    platform=freebsd
    QMAKE_OPT="$QMAKE_OPT CONFIG+=freebsd"
else
    export MAKE=make
    echo "Platform is Linux"
    platform=linux
    QMAKE_OPT="$QMAKE_OPT CONFIG+=linux"
fi


# usually no need to change anything below this line...

if [ -z $QTDIR ]
then
    echo "QTDIR not set. Aborting.."
    exit
fi

modules="qcadlib qcadcmd qcadactions qcadguiqt"

for arg in $@
do
    if [ "$arg" == "noclean" ] ; then
        noclean="true"
    fi
    if [ "$arg" == "noconfig" ] ; then
        noconfig="true"
    fi
    if [ "$arg" == "noprepare" ] ; then
        noprepare="true"
    fi
    if [ "$arg" == "notrans" ] ; then
        notrans="true"
    fi
    if [ "$arg" == "distcc" ] ; then
        # look for hosts for distributed compilation:
        if [ ! -z "$DISTCC_HOSTS" ]
        then
            export MAKE="$MAKE -j8 CXX=distcc"
            echo "Using distcc with hosts $DISTCC_HOSTS"
        fi
    fi
done


if [ "x$platform" == "xosx" ]
    then
        targetname="QDraw"
    else
        targetname="qdraw"
fi


target=$targetname$targetpostfix

echo "Target is $target"

if [ -f ../qcad/$target ]
then
    echo "Removing previous target"
    rm ../qcad/$target
fi

# clear demo-specific objects
if [ "x$noclean" == "x" ]
then
    rm -f ../qcad/src/obj/main.o*
    rm -f ../qcad/src/obj/qc_applicationwindow.o*
fi


echo "QMAKE_OPT: $QMAKE_OPT"

cd ..

echo "-------- Building fparser --------"
cd fparser
if [ -z $noconfig ]
then
    ./configure
fi
eval $MAKE
cd ..

if [ ! -f fparser/lib/libfparser.$libext ]
then
    echo "Building libfparser failed"
    exit;
fi

echo "-------- Building dxflib --------"
cd dxflib
if [ -z $noconfig ]
then
    ./configure
fi
eval $MAKE
cd ..

if [ ! -f dxflib/lib/libdxf.$libext ]
then
    echo "Building libdxf failed"
    exit;
fi


if [ -z $noprepare ]
then
    for p in $modules
    do
        if [ -d $p ]
        then
            echo "-------- Preparing $p --------"
            cd $p
            cd src
            qmake $p.pro $QMAKE_OPT
            cd ..
            eval $MAKE prepare
            cd ..
        fi
    done
fi

for p in $modules
do
    if [ -d $p ]
    then
        echo "-------- Building $p --------"
        cd $p
        cd src
        qmake $QMAKE_OPT $p.pro 
        if [ "x$platform" == "xwin32" ]
        then
            eval $NMAKE
            cd ..
        else
            cd ..
            eval $MAKE
        fi
        cd ..

        if [ ! -f $p/lib/*.$libext ]
        then
            echo "Building $p failed"
            exit;
        fi
    fi
done

echo "-------- Building QCad --------"
cd qcad
cd src
qmake qcad.pro $QMAKE_OPT
if [ "x$platform" == "xwin32" ]
then
    evel $NMAKE
    cd ..
else
    cd ..
    eval $MAKE  
fi

cd ..

if [ "x$platform" == "xosx" ]
then
    if [ ! -d qcad/$target.app ]
    then
        echo "Building qdraw binary failed"
        exit;
    fi
else
    if [ ! -f qcad/$target ]
    then
        echo "Building qdraw binary failed"
        exit;
    fi
fi

# create mac .app folder:
cd ./qcad
if [ "x$platform" == "xosx" ]
then
    if [ -d $target.app ]
    then
        echo Preparing Application Folder for $target
    fi

    strip $target.app/Contents/MacOS/$target

    #mkdir $target.app/Contents/Resources/doc
    #cp doc/*.html doc/*.css doc/*.adp $target.app/Contents/Resources/doc/
    #mkdir $target.app/Contents/Resources/doc/img
    #cp doc/img/*.png $target.app/Contents/Resources/doc/img/
    #mkdir $target.app/Contents/Resources/doc/imgs
    #cp doc/img/*.png $target.app/Contents/Resources/doc/imgs/
    mkdir $target.app/Contents/Resources/qm
    cp qm/*.qm $target.app/Contents/Resources/qm/
    mkdir $target.app/Contents/Resources/fonts
    cp fonts/*.cxf $target.app/Contents/Resources/fonts/
    mkdir $target.app/Contents/Resources/patterns
    cp patterns/*.dxf $target.app/Contents/Resources/patterns/
    #cp src/Info.plist $target.app/Contents/

    if [ -d "$QTDIR/bin/assistant.app" ]
    then
         mkdir -p "$target.app/Contents/MacOS/bin"
         cp -R "$QTDIR/bin/assistant.app" "$target.app/Contents/MacOS/bin/"
    fi
fi

#cd scripts
#bash ./release_translations.sh

