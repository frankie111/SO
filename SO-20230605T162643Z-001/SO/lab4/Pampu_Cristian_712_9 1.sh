#!/bin/bash

folder_path="$1"
function_name="$2"
value_to_replace="$3"

# Find all .c files in the folder
files=$(find "$folder_path" -type f -name "*.c")

# Initialize the counter for the number of replacements
replacements=0

# Iterate through each file found
for file in $files; do
  # Use a sed expression to replace the function parameter with the specified value
  # and count the number of replacements made
  replacements_in_file=$(sed -n "s/\($function_name\s*(\)\s*[0-9]*\s*,/\1$value_to_replace,/gp" "$file" | wc -l)

  # Update the global replacements counter
  replacements=$((replacements + replacements_in_file))

  # Edit the original file with the sed command
  sed -i "" "s/\($function_name\s*(\)\s*[0-9]*\s*,/\1$value_to_replace,/" "$file"
done

# Print the total number of replacements made
echo "Replacements made: $replacements"
