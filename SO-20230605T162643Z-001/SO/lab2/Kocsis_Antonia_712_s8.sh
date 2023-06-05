#!/bin/bash

# Check if file name is provided as argument
if [ $# -eq 0 ]; then
  echo "Filename needed"
  exit 1
fi

# Use sed command to replace the third word with the first word in each line of the file
# The -r option enables extended regular expressions (ERE)
# The s command replaces the third word (using a capture group) with the first word
# The \1 and \3 in the replacement string refer to the captured first and third words, respectively
# The g modifier at the end of the s command makes the substitution global (i.e., on all matches in the line)
sed 's/\([^:]*\):\([^:]*\):\([^:]*\)/\1:\2:\1/g' "$1"

