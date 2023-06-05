#!/bin/bash

if [ $# -ne 6 ]; then
    echo "Error: Six arguments are required."
    exit 1
fi

num1=$1
num2=$4
sum1=$((num1 * num2))

num1=$2
num2=$5
sum2=$((num1 * num2))

num1=$3
num2=$6
sum3=$((num1 * num2))

sum4=$((sum1 + sum2 + sum3))

if [ $sum4 -eq 0 ]; then
    echo "Vectors are perpendicular"
else
    echo "Vectors are not perpendicular"
fi