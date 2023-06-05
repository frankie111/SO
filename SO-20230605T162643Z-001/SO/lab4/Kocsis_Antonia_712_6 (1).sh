#!/bin/bash

if [ $# -ne 1 ]; then
  echo ""
  exit 1
fi

folder_name=$1

if [ ! -d "$folder_name" ]; then
  echo "Error: $folder_name is not a directory"
  exit 1
fi

find "$folder_name" -depth -mindepth 1 | sort | uniq -c | awk '{print $2, $1}' | awk -F'/' '{print $NF, $1}' | awk '{a[$1]+=$2} END {for (i in a) {print i, a[i]}}'

