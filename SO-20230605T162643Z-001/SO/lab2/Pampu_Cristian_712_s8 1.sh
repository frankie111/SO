#!/bin/bash

# if statement checks if the first argument ($1) is empty
# or if the number of arguments ($#) is not equal to 1
if [ -z "$1" ] || [ $# -ne 1 ]; then
  echo "Usage: $0 filename"
  exit 1
fi

# matches one or more characters that are not colons
# and captures them as groups
# we only care about the first 3 groups
# we will replace group 3 with group 1
# we dont edit the original file
sed -E 's/^([^:]+):([^:]+):([^:]+)/\1:\2:\1/' "$1"
