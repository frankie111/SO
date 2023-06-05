#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Please provide a file name."
  exit 1
fi

file=$1
sum=$(awk '{for (i=1; i<=NF; i++) sum+=$i} END{print sum}' RS='[^0-9]+' "$file")
num_lines=0
num_lines_with_numbers=0

while read line; do
  if echo "$line" | awk '/[0-9]+/{found=1} END{exit !found}'; then
    num_lines_with_numbers=$((num_lines_with_numbers + 1))
  fi
  num_lines=$((num_lines + 1))
done < "$file"

if [ $num_lines -gt 0 ]; then
  ratio="$num_lines_with_numbers:$((num_lines - num_lines_with_numbers))"
else
  ratio="0:0"
fi

echo "$sum $ratio"
