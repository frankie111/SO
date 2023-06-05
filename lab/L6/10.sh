#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Please provide at least one name"
    exit 1
fi

for name in "$@"
do
    filename="$name.txt"    
    if [ -f "$filename" ]
    then
        num1=$(awk 'NR==1{print $1}' "$filename")
        num2=$(awk 'NR==2{for(i=1;i<=NF;i++)s+=$i;print s;}' "$filename")
        result=$(echo "$num2*6" | bc)
        division=$(echo "scale=2; $result/$num1" | bc)
        printf "\n%s %.1f\n" "$result" "$division" >> "$filename"
    else
        echo "$filename not found"
    fi
done
