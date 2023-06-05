#!/bin/bash

if [ -z "$1" ]
then
  echo "Directory name not provided"
  exit 1
fi

dir_path="$1"

ls -1v "$dir_path"
echo 
ls -ltr "$dir_path"
echo
ls -lS "$dir_path"
