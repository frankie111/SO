#!/bin/bash

# Check if the number of arguments is correct
if [ $# -ne 1 ]; then
    echo "Incorrect number of arguments."
    echo "Usage: $0 [manufacturer or model]"
    exit 1
fi

# Save the search pattern in a variable
search=$1

# Use grep to search for the pattern in the stoc.csv file
# Use grep -v to invert match (exclude)
# a comma followed by a space and a 0 at the end of the line.
grep -i "$search" stoc.csv | grep -v ", 0$" | while read line; do
    # Output the matching lines
    echo "$line"
done
