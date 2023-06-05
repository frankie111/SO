#!/bin/bash

if [ -z "$1" ]; then
  echo "Please provide a file name"
  exit 1
fi

if [ ! -f "$1" ]; then
  echo "File not found"
  exit 1
fi

sed -r 's/^([a-zA-Z]+)([^a-zA-Z]+[a-zA-Z]+[^a-zA-Z]+)([a-zA-Z]+)(.*)/\3\2\1\4/' "$1"

#sed -r 's/^([a-zA-Z]*)([^a-zA-Z]+[a-zA-Z]+[^a-zA-Z]+)([a-zA-Z]*)(.*)/\3\2\1\4/' "$1"
