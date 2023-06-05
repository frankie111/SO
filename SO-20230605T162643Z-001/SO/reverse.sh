#!/bin/bash

# Check if the file is provided
if [ $# -eq 0 ]; then
    echo "No file provided. Usage: $0 <filename>"
    exit 1
fi

# Check if the file exists
if [ ! -f $1 ]; then
    echo "File $1 does not exist."
    exit 1
fi

# Reverse the lines of the file and write to reversed.txt
# ONLY ON MAC OS!
tail -r $1 > reversed.txt

# !!! USE THIS ON UBUNTU !!!
# tac $1 > reversed.txt

#Create a shell script that receives a text file as a command line argument. The script creates a new file named "reversed.txt" that contains the reversed lines of the input file. The script should reverse the order of the lines from the input file and write them into "reversed.txt", without changing the original file. The reversed lines should be displayed in the standard output.
#Example:
#input:
#"example.txt"
#Hello, how are you today?
#I am doing well.
#This is an example file.
#Task 2 (C-Programming)
#output:
#"reversed.txt":
#This is an example file. I am doing well.
#Hello, how are you today?