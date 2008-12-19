#!/bin/bash
# $Id: build_qcad.sh 2488 2005-07-20 08:33:25Z andrew $

echo "build_qcad.sh"
echo "Usage: ./build_qcad.sh [options]"
echo "options:"
echo "  demo        build demo version"
echo "  debug       add debug menu"
echo "  cam         build CAM support if available"
echo "  scripting   build scripting support if available"
echo "  prof        build professional version"
echo "  noclean     don't clean (speeds up building if the options don't change)"
echo "  noconfig    don't run configure (speeds up building if the options don't change)"
echo "  noprepare   don't run prepare (speeds up building if the options don't change)"
echo "  notrans     don't generate translations"
echo "  distcc      use distcc for distributed compilation. DISTCC_HOSTS must be set."
echo

echo "QTDIR is: $QTDIR"
echo "QMAKESPEC is: $QMAKESPEC"

# detect system:
if [ "x$OS" == "xWindows_NT" ]
then
    export MAKE=MinGW32-make
    echo "Platform is Windows"
    platform=win32
elif [ `uname` == "SunOS" ]
then
    export MAKE=gmake
    echo "Platform is Solaris"
    platform=solaris
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
else
    export MAKE=make
    echo "Platform is Linux"
    platform=linux
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
    if [ "$arg" == "demo" ] ; then
        QMAKE_OPT="$QMAKE_OPT CONFIG+=rs_demo"
        rs_demo="true"
    fi
    if [ "$arg" == "debug" ] ; then
        QMAKE_OPT="$QMAKE_OPT DEFINES+=RS_DEBUGGING"
    fi
    if [ "$arg" == "cam" ] ; then
        QMAKE_OPT="$QMAKE_OPT CONFIG+=rs_cam"
        modules="$modules qcadcam"
        rs_cam="true"
    fi
    if [ "$arg" == "scripting" ] ; then
        QMAKE_OPT="$QMAKE_OPT CONFIG+=rs_scripting"
        modules="$modules qcadscripting"
    fi
    if [ "$arg" == "prof" ] ; then
        QMAKE_OPT="$QMAKE_OPT CONFIG+=rs_prof"
        modules="$modules qcadprof"
    fi
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


if [ "x$rs_cam" == "xtrue" ]
then
    if [ "x$platform" == "xosx" ]
    then
        targetname="CAMExpert"
    else
        targetname="camexpert"
    fi    
else
    if [ "x$platform" == "xosx" ]
    then
        targetname="QCad"
    else
        targetname="qcad"
    fi    
fi
    
if [ "x$rs_demo" == "xtrue" ]
then
    if [ "x$platform" == "xosx" ]
    then
        targetpostfix="Demo"
    else
        targetpostfix="_demo"
    fi    
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

if [ ! -f fparser/lib/libfparser.a ]
then
    echo "Building libfparser.a failed"
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

if [ ! -f dxflib/lib/libdxf.a ]
then
    echo "Building libdxf.a failed"
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
        qmake $p.pro $QMAKE_OPT
        if [ "x$platform" == "xwin32" ]
        then
            sed "s/\\\\/\//g" Makefile > tmp2
            sed "s/ \// \\\\/g" tmp2 > tmp3
            mv tmp3 Makefile
        fi
        cd ..
        eval $MAKE
        cd ..

        if [ ! -f $p/lib/*.a ]
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
    sed "s/\\\\/\//g" Makefile > tmp2
    sed "s/ \// \\\\/g" tmp2 > tmp3
    sed "s/\"\/\"/\"\\\\\"/g" tmp3 > tmp2
    sed "s/\/\"\"/\\\\\"\"/g" tmp2 > tmp3
    mv tmp3 Makefile
fi
cd ..


eval $MAKE
cd ..

if [ -z $notrans ]
then
    echo "-------- Building Translations --------"
    cd scripts
    sh ./release_translations.sh
fi

if [ "x$platform" == "xosx" ]
then
    if [ ! -d qcad/$target.app ]
    then
        echo "Building qcad binary failed"
        exit;
    fi
else
    if [ ! -f qcad/$target ]
    then
        echo "Building qcad binary failed"
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

    if [ "x$rs_cam" == "true" ]
    then
        echo "Copying machine configuration files"
        mkdir $target.app/Contents/Resources/machines
        cp machines/*.cxm $target.app/Contents/Resources/machines/
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

