#!/bin/bash

#Create a shell script that takes a text file name as a command line argument.
# The script should count the number of lines in the file that contain a specific word or phrase, and display the results in the standard output.
#Example:
#input:
#"example.txt"
#Hello, how are you? I'm fine.
#And you?
#I'm fine too, thank you.
#Search for: "how are"

#output:
#The phrase "how are" appears in 3 lines.
#./solution.sh "example.txt" "how are"

# Ensure that two arguments have been provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <filename> <search_string>"
    exit 1
fi

# Define the filename and search string variables from the provided arguments
filename=$1
search_string=$2

# Use grep to find the lines that contain the search string and count them with wc -l
line_count=$(grep -o "$search_string" "$filename" | wc -l)

# Print the results
echo "The phrase \"$search_string\" appears in $line_count lines."
