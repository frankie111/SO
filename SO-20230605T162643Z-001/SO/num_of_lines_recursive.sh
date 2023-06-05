#!/bin/bash

# Check if directory exists
if [ ! -d "$1" ]
then
    echo "Directory $1 does not exist."
    exit 1
fi

# Initialize line counter
line_count=0

# The "while IFS= read -r file" line will read each line outputted by the find command.
# IFS= is used here to prevent leading/trailing whitespace in file names from being trimmed.
while IFS= read -r file
do
    # wc -l <"$file" counts the number of lines in the current file.
    # The $(...) syntax is called command substitution. It runs the command inside the parentheses,
    # and replaces the $(...) with the output of that command.
    # This value is then added to the line_count variable.
    line_count=$((line_count + $(wc -l <"$file")))
# The <(find ...) syntax is called process substitution. It runs the find command inside the parentheses,
# and replaces <(find ...) with a file descriptor that points to a file containing that command's output.
# So, the while loop takes its input from this file descriptor.
# Note that we cannot use < $(find ...) here. The $(...) syntax alone performs command substitution,
# replacing $(...) with the output of the command, which is not what we want. The < operator expects a file name,
# not raw command output. Hence, we use process substitution <(command) instead of command substitution $(command).
done < <(find "$1" -name "*.txt" -type f)

# At this point, we've gone through each .txt file in the specified directory and all its subdirectories,
# adding up the line counts for each file.

# Print total line count
echo "Total number of lines: $line_count"


#Create a shell script that takes a directory as a command line argument. The script should recursively search for text files in the directory and display the total number of lines across all found text files.
#Example:
#input:
#Assuming the following structure:
#directory/
#├── file1.txt
#├── file2.txt
#├── file3.c
#└── subdirectory/
#    ├── file3.txt
#    └── subsubdirectory/
#        └── file4.txt
#
#output:
#Total number of lines: 32