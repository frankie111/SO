#!/bin/bash

dir=$1

if [ ! -d "$dir" ]; then
    echo "Invalid folder path."
    exit 1
fi

txt_files=$(find "$dir" -type f -name "*.txt")

total=0

for file in $txt_files; do
    lines=$(wc -l < "$file")
    total=$((total + lines))
done

echo $total