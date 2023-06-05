#!/bin/bash
if [ $# -lt 2 ]; then
echo "Needs at least 2 parameters"
fi
for file in "$@"
do
	if file "$file" | grep -q "shell script"
then
	echo "$file"

	fi 
done | sort 
echo

