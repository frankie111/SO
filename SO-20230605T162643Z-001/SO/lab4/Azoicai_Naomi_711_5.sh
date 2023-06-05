#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Please enter two Ordners"
    exit 1
fi

if [ $# -eq 1 ]; then
    echo "Please enter one more Ordner"
    exit 1
fi

first=$1
second=$2
mv "$second" "$first/"

for file in "$first/$second"/*; do
    if [ -d "$file" ]; then
        for subfile in "$file"/*; do
            if [ -f "$subfile" ]; then
                new_file=$(echo "$subfile" | awk -F '.' '{print $1 ".eins"}')
                mv "$subfile" "$new_file"
            fi
        done
    elif [ -f "$file" ]; then
        new_file=$(echo "$file" | awk -F '.' '{print $1 ".eins"}')
        mv "$file" "$new_file"
    fi
done

