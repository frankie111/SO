#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 <file1> [<file2> ...]"
    exit 1
fi

for filename in "$@"; do
    awk '{
            delete cnt;
            for (i = 1; i <= NF; i++) {
                if ($i in cnt) {
                    cnt[$i]++;
                } else {
                    cnt[$i] = 1;
                }
            }
            for (word in cnt) {
                if (cnt[word] > 1) {
                    print FILENAME, cnt[word], word;
                }
            }
    }' "$filename"
done
