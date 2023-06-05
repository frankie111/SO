#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Please provide 3 parameters: directory function replaceparameter"
    exit 1
fi

dir=$1
fun=$2
par=$3

if [ ! -d "$dir" ]
then
    echo "Please provide a valid directory"
    exit 1
fi

for file in $dir/*.c
do
    if [ -f "$file" ]
    then
        sed -i "s/\($fun\)([^)]*)/\1($par)/g" $file
    fi
done
