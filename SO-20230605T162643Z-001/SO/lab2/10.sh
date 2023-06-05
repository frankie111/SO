#!/bin/bash

if [ -z "$1" ]; then
  echo "Enter name for file"
  exit 1
fi

if [ ! -f "$1" ]; then
  echo "no file found"
  exit 1
fi

sed -r 
's/^([a-zA-Z]+)([^a-zA-Z]+[a-zA-Z]+[^a-zA-Z]+)([a-zA-Z]+)(.*)/\3\2\1\4/' 
"$1"

