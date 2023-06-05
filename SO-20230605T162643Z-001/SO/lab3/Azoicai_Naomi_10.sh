#!/bin/bash

if [ $# -eq 0 ]; then
        echo "Please insert file names next time!"
        exit 1
fi

for file in "$@"; do
    awk '
        BEGIN { FS="[ .!?]" }
        {
            for (i=1; i<=NF; i++) {
                if (words[$i]) {
                    print FILENAME, NR, $i;
                    break;
                }
                words[$i]++;
            }
            delete words;
        }
    ' "$file"
done
