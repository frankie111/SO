#!/bin/bash

if [ -z "$2" ]
then
  echo "Needs second parameter."
  exit 1
fi

dirs=$1
files=$2

if [ -d "ex8" ];
  then rm -r ex8/*;
fi

for ((i=1;i<=$dirs;i++)); do
    dir_name="dir_$i"
    mkdir -p ex8
    mkdir -p ex8/$dir_name
    for ((j=1;j<=$files;j++)); do
        file_name="file_$j.txt"
        touch ex8/$dir_name/$file_name
    done
done