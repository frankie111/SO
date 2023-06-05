#!/bin/bash

dir=$1

if [ ! -d "$dir" ]; then
    echo "Enter a valid directory"
    exit 1
fi

files=$(ls | grep *.c < $dir)

echo "$files"

