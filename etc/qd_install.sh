#!/bin/sh

echo 'Do you agree with the QD-2.3.24 copyright licensing?'
echo 'Please read carefully the license document.'
echo 'https://www.davidhbailey.com/dhbsoftware/LBNL-BSD-License.docx'

read -p "ok? (y/N): " yn
yn=`echo $yn'N'|cut -c 1-1`
ok=0
case "$yn" in
  [yY]*)ok=1;;
esac
if [ $ok -eq 0 ]; then
	echo 'Benchmark code needs the latest qd library.'
        echo 'Please install it by yourself appropleately.'
	echo 'Technical details are on https://www.davidhbailey.com/dhbsoftware/'
	echo 'If you already have installed on your system, link the qd directory to qd_real.'
	echo 'For example, ln -s (qd-directory) ./qd_real'
	exit 1
fi


if [ -f qd-2.3.24.tar.gz ]; then
	\rm qd-2.3.24.tar.gz
fi 
wget https://www.davidhbailey.com/dhbsoftware/qd-2.3.24.tar.gz


if [ -f qd-2.3.24 ]; then
	\rm -rf qd-2.3.24
fi
tar zxvf qd-2.3.24.tar.gz
cd qd-2.3.24
./configure --enable-ieee-add --disable-sloppy-mul --disable-sloppy-div
make
cd ..


if [ -e ./qd_real ]; then
	\rm qd_real
fi
ln -s qd-2.3.24 ./qd_real

