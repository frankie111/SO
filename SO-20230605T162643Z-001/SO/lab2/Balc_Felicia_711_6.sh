#!/bin/bash

file_name=$(grep -v '^#' "$1")
shift

for dir in "$@"
do
    find "$dir" -type f -name "$file_name" -delete
done
