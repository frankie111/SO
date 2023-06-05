#!/bin/bash

if [ $# -lt 2 ]; then
    echo "provide a word and filenames"
    exit 1
fi

word="$1"
shift

count=0

for file in "$@"; do
    if grep -q "$word" "$file"; then
        echo "$file"
        count=$((count+1))
    fi
done

echo "$count"
