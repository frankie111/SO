#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Gibt 1 und nur 1 Datei!"
	exit 1
fi

file=$1
dirname=Worterbuch

# Delete old directory and create new
if [[ ! -d "$dirname" ]]
then
	mkdir ./$dirname
else
	rm -r "$dirname"
	mkdir ./$dirname
fi


for i in {A..Z}
do
	touch ./$dirname/$i
	awk '{for (i=1; i<=NF; i++) print $i}' datei | grep -i "^$i" >> ./$dirname/$i
done

for file in "$dirname"/*
do
	if [ -f "$file" ]
	then
		sort -uf "$file" -o "$file"
	fi
done

