#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Please provide a directory."
  exit 1
fi

path="$1"

if [ ! -d "$path" ]; then
  echo "$path is not a directory."
  exit 1
fi

grep -lE'*.txt' "$path"
