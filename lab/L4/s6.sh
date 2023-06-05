#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Please provid two parameters"
  exit 1
fi

symbol="$1"
file="$2"

if [ ! -f "$file" ]; then
  echo "Please provide a file"
  exit 1
fi

sed "s/[a-z]/$symbol/g" "$file"
