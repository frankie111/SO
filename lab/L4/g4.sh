#!/bin/bash

if [ -z "$1" ]; then
  echo "Please provide a folder name"
  exit 1
fi

if [ ! -d "$1" ]; then
  echo "Folder not found"
  exit 1
fi

find "$1" -type f -exec grep -Iq . {} \; -print | sort
