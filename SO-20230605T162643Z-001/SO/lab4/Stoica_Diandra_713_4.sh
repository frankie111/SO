#!/bin/bash

if [ $# -lt 2 ]
then
echo "not enough parameters"
exit 1
fi

folder="$1"
shift
filesr="$@"

for file in $files
do
echo -n  "$file was found in: "
echo "$(find "$folder" -name "$file" | grep -o "/.*")"
done



