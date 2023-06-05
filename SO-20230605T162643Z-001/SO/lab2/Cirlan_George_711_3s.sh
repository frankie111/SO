#!/bin/bash

if [ $# -lt 2 ]
then
	echo "Nicht genug Parameter!"
	exit 1
fi

text="$1"
shift

for file in "$@"
do
	sed -e "1,30{/^.*${text}.*$/d;}" "$file"
done

exit 0
