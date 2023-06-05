#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file1> [<file2> ...]"
    exit 1
fi

for filename in "$@"; do
    awk '{
            delete count;
            for (i = 1; i <= NF; i++) {
                if ($i in count) {
                    count[$i]++;
                } else {
                    count[$i] = 1;
                }
            }
            for (word in count) {
                if (count[word] > 1) {
                    print FILENAME, count[word], word;
                }
            }
    }' "$filename"
done