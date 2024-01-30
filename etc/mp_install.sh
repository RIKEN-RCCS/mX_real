#!/bin/sh

echo 'Do you agree with the mpreal copyright licensing?'
echo 'It is GNU GENERAL PUBLIC LICENSE Version 3,'

read -p "ok? (y/N): " yn
yn=`echo $yn'N'|cut -c 1-1`
ok=0
case "$yn" in
  [yY]*)ok=1;;
esac
if [ $ok -eq 0 ]; then
	exit 0
fi

mkdir mpreal
git clone https://github.com/advanpix/mpreal.git

