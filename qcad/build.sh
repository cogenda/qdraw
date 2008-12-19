#! /bin/sh

rm debug_*.log
rm debug_*.html

#export QTDIR=/usr/lib/qt3
#export QTDIR=/opt/qt

export QTDIR=/opt/qt_static
export QMAKESPEC=/home/andrew/data/RibbonSoft/projects/QCad2/mkspecs/linux-g++-qcad

#export LD_LIBRARY_PATH=$QTDIR/lib

base="/home/andrew/data/RibbonSoft/projects/QCad2"

start=`date`

echo "--------------------------------------------------------------- dxflib"
cd $base/dxflib
make
#make install

for pro in qcadlib qcadcmd qcadcam qcadactions qcadguiqt qcadscripting
do
	echo "--------------------------------------------------------------- $pro"
	cd $base/$pro/src
	$QTDIR/bin/qmake ./$pro.pro -o ./Makefile
	make staticlib
	cd ..
	#make install
done

echo "--------------------------------------------------------------- qcad"
#read

cd $base/qcad/src
qmake -o Makefile qcad.pro
rm ../qcad
make

stop=`date`

echo "$start - $stop"

