#!/bin/bash

# Check that a directory was provided
if [ -z "$1" ]
then
    echo "Please provide a directory as an argument."
    exit 1
fi

# Initialize line counter
total_lines=0

# Loop over all files in the directory
for file in "$1"/*
do
    # If it is a file, count the lines
    if [ -f "$file" ]
    then
        total_lines=$((total_lines + $(wc -l < "$file")))
    fi
done

# Print the total line count
echo "Total lines: $total_lines"


#  [ –d  file ] 		# if file is a directory.
#  [ –e  file ] 		# if file exists.
#  [ –f  file ] 		# if file exists and is a regular file.
#  [ –L  file ] 		# if file is a symbolic link.
#  [ –r  file ] 		# if file is a readable file.
#  [ –w  file ] 		# if file is a writeable file.
#  [ –x  file ] 		# if file is an executable file.