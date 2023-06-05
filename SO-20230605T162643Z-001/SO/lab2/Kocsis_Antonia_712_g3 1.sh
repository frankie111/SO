#!/bin/bash

# Check if directories are provided as arguments
if [ $# -eq 0 ]; then
  echo "Usage: $0 directory1 [directory2 ...]"
  exit 1
fi

# Loop through all directories provided as arguments
for dir in "$@"; do
  # Use the find command to search for all files in the directory that are not ASCII text files
  # The -type f option specifies that we are looking for regular files, not directories
  # The -exec option executes the file command on each file found
  # The {} + option tells find to replace the '{}' with the list of file names that match the search criteria
  binary_files=$(find "$dir" -type f -exec file {} + | grep -c -v ASCII)

  # If the number of binary files is greater than zero, print the directory name and the file names
  if [ $binary_files -gt 0 ]; then
    echo "Binary files found in directory $dir:"
    find "$dir" -type f -exec file {} + | grep -v ASCII | awk -F: '{print "\t"$1}'
  fi
done

