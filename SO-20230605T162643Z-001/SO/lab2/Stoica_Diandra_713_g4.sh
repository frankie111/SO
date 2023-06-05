#!/bin/bash

if [ $# -lt 1 ]
then
echo "not enough parameters"
exit 1
fi

directory=$1

find "$directory" -type f -exec file {} + | grep -w "ASCII text" | cut -d ':' -f 1 | sort


