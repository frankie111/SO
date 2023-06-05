#!/bin/bash

if [ -z "$1" ]; then
  echo "Please provide at least one file name"
  exit 1
fi

file="$1"

sed 's/\b[[:alnum:]]*[[:digit:]][[:alnum:]]*\b//g' "$file"
