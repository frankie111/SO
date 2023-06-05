#!/bin/bash

if [ $# -lt 1 ]
then
echo "Not enough Parameters"
exit 1
fi

directory=$1

for file in `ls $directory`
do
echo `file $file`| grep 'ASCII' | cut -d ':' -f 1
done
