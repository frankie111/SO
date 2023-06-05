#!/bin/bash

if [ $# -eq 0 ]
then
	echo "Pass nicht!"
	exit 1
fi

for f in "$@"
do
	awk '
		BEGIN { FS="" }
		{	
			for( i=1; i<=NF; i++){
				if($i ~ /[aeiouAEIOU]/) 
					countv++
				else{

					if($i ~ /[a-zA-Z]/)
						countc++
				}
			}
		}
		END{print FILENAME, "", countv, countc}
	' "$f"
done

