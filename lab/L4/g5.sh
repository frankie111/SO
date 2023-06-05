#!/bin/bash
if [ -z "$1" ]
then
  echo "provide a directory"
  exit 1
fi

if [ ! -d "$1" ]
then
  echo "provide an actual directory"
  exit 1
fi

find "$1" -type f -name "*.c" -exec sh -c 'if [ "$(grep -c "#include" "$0")" -gt 3 ]; then echo "$1"; fi' {} \;
