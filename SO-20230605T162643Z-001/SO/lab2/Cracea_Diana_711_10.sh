#!/bin/bash
if [ $# -eq 0 ]; then
	echo "Missing file name"
	exit 1
fi
sed 's/\([^ ]*\) \(.*\) \([^ ]*\) \(.*\)/\3 \2 \1 \4/' "$1"

