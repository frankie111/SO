#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Please insert file name next time!"
	exit 1
fi

sed 's/\([^:]*\):\([^:]*\):[^:]*/\1:\2:\1/g' "$1"
