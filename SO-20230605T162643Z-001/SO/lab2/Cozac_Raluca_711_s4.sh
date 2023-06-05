#!/bin/bash

if [ $# -ne 2 ]; then
echo "Wrong nr of parameters"
exit 1
fi

word="$1"
file_path="$2"


sed -E "s/([a-z])/$word\1/g" "$file_path"
