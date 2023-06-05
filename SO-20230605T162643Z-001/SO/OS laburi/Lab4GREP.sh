#!/bin/bash
text=$1
if [ "$#" -lt 2 ]; then
echo "Not enough parameters"
fi
shift
for file in "$@"; do
	sed -s "1,30{/.*$text.*/d}" "$file"
done

