#!/bin/bash

input_file="$1"
output_file="reversed.txt"

# Use tac to reverse the order of lines and store the result in output_file
tac "$input_file" > "$output_file"

# Display the reversed lines in the standard output
tac "$input_file"