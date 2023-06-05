#!/bin/bash

if [ $# -lt 1 ]
then
echo "not enough parameters"
exit 1
fi

for file in $@
do
awk '{a[NR]=$0} END {for(i=NR;i>=1;i--) print a[i]}' $file
done

