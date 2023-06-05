#!/bin/bash

if [ -z "$1" ]; then
  echo "Please provide at least one file name"
  exit 1
fi

grep -lE "*.sh" "$@" | sort
