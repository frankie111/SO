#!/bin/bash

# Loop through each input file passed as command-line argument
for file_path in "$@"
do

  # In awk NF = number of fields (words) in the current record (line)
  #        NR = current record (line)
  #        FILENAME = name of the current input file being processed
  # Use awk to search for repeated words in the current file

  awk '
  {
    # Loop through each word in the current line
    # $1 refers to the first field (word), $2 to the second, etc..
    for(i=1; i<=NF; i++){
      # Keep track of the number of occurrences of each word
      word_count[$i]++

      # If the current word has occurred twice, print the filename, line number, and word
      if(word_count[$i] == 2){
        print FILENAME, NR, $i
      }
    }
  }
  ' "$file_path"
done
