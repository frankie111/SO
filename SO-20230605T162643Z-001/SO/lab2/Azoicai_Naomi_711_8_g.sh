#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Please insert file names next time!"
	exit 1
fi

for file in "$@"; do
	if echo "$file" | grep -qE '\.sh$'; then
	echo "$file"
	fi
done | sort
