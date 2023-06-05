#!/bin/bash
datei=$1
shift
if [ $# -ge 1 ]; then
for ordner in $@
do
for file in `ls $ordner`
do
	if [[ -d $file ]]
	then
	for  filename in `ls $file`
		do
		if [ `grep -c "$filename" "$datei"` -ge 1 ]; then
		rm $ordner'/'$file'/'$filename
		fi
	done
	fi
done
done
fi
