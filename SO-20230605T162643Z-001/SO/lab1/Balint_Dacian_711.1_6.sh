#!/bin/bash

if [ $# -ne 6 ]
then
echo "6 Zahlen mussen angegeben"
exit 1
fi

x1=$1
y1=$2
x2=$3
y2=$4
x3=$5
y3=$6

area=$(( ($x1*($y2-$y3)+$x2*($y3-$y1)+$x3*($y1-$y2))/2 ))

if [ $area -lt 0 ]
then
area=$(( $area*-1 ))
fi


if [ $area -eq 0 ]
then
echo "kollinear"
else
echo $area
fi
