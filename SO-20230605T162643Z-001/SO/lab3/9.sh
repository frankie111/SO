#!/bin/bash

if [ $# -lt 3 ]; then
  echo "Usage: $0 folder function_name new_value"
  exit 1
fi

folder=$1
function_name=$2
new_value=$3

# Count the number of replacements made
count=0

# Use find command to search for .c files in the given folder
# and loop through each file found
find "$folder" -name "*.c" | while read filename
do
  # Use grep to find all lines containing the function call
  # and use sed to replace the parameter with the new value
  # count the number of replacements made
  grep -n "$function_name(" "$filename" | while read -r line; do
    line_number=$(echo "$line" | cut -d ':' -f 1)
    line_content=$(echo "$line" | cut -d ':' -f 2-)
    sed -i "${line_number}s/\($function_name(\)[^)]*/\1$new_value/" 
"$filename"
    count=$((count+1))
    echo "Replaced $line_content with $function_name($new_value) in 
$filename"
  done
done

echo "Total replacements made: $count"

