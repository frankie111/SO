#!/bin/bash
echo "Dateien mit Schreibrechten fuer "others"":
find . -type f -perm /o=w | while read -r file; do
	echo "$file"
done
