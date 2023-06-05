#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Bitte geben Sie den Ordnernamen ein!"
	exit 1
fi

for f in "$@"
do
	if file "$f" | grep -q  "ASCII text";
	then
		echo "$f"
	fi
done | sort

exit 0
