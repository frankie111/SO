#!/bin/bash
if [ $# -lt 1 ]
then
	echo "Kein Datei gegeben"
	exit 1
fi

if [ ! -d "Worterbuch" ]
then
	mkdir Worterbuch
fi

cd Worterbuch
for i in {65..90}
do
	touch $(printf "\x$(printf %x $i)").txt
done
cd ..
for word in `cat $1 | tr ' ' '\n' | sort -f`
do
	cd Worterbuch
	uppercase_word=${word^}
	filename="${uppercase_word:0:1}.txt"
	printf "%s\n" $word >> $filename
	cd ..
done
