#!/bin/bash

total_words=0  #initialize total number of words for all files to 0
total_files=0  #initialize total number of files to 0

for file in "$@"; do  #loop through each input file
    if [[ -f "$file" ]]; then  #check if it's a file
        words=$(awk '{for (i=1; i<=NF; i++) {count++}} END {print count}' "$file") #count number of words in file
        lines=$(awk 'END {print NR}' "$file")  #count number of lines in file
        median=$((words / lines)) #calculate median number of words per line
        echo "$file $median"  #print filename and median number of words
        ((total_words += words))  #add number of words to total
        ((total_files += 1))  #increment file count
    fi
done

average=$((total_words / total_files))  #calculate average number of words per file
echo "Average words per file: $average"  #print average

