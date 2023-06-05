#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Bitte geben Sie den Dateinamen als Argument an."
  exit 1
fi

file=$1

sed -i y/$(printf "%s" {a..z})/$(printf "%s" {f..z} {a..e})/ $file

echo "gata"