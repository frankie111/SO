#!/bin/bash

# Check if folder name is provided as argument
if [ $# -eq 0 ]; then
    echo "Please provide folder name as argument."
    exit 1
fi

# Initialize total lines count to zero
total_lines=0

# Iterate over all text files in folder and subdirectories
for file in $(find "$1" -type f -name "*.txt"); do
    # Add number of lines in current file to total count
    current_lines=$(wc -l < "$file")
    total_lines=$((total_lines + current_lines))
done

# Display the total number of lines
echo $total_lines
