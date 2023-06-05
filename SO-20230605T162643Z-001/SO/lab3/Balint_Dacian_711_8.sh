#!/bin/sh

if [ $# -lt 1 ]
then
echo "The name of the datafile was not given"
exit 1
fi


awk 'BEGIN{ number=0 
nonumber=0
sum=0 } {
for(i=1;i<=NF;i++) {
	if ($i ~ /[0-9]+/) {
	sum+=$i
	contains=1
	}
	}
	if(contains) {
	number++
	contains=0
        }
	else {
	nonumber++
}
 } END { print sum, number ":" nonumber }' $1
