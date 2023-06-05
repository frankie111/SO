#!/bin/bash
a=$1
b=$2
c=$3
p=1
i=2

if [ $# -ne 3 ]
then
	echo "write 3 numbers"
	exit 1
fi
while [ $i -le $a ]
do
        rest_a=$(( a % i ))
        rest_b=$(( b % i ))
        rest_c=$(( c % i ))
        mark_a=0
        mark_b=0
        mark_c=0
        if [ $rest_a -eq 0 ]
        then
                mark_a=1
                a=$(( a / i ))
        fi
        if [ $rest_b -eq 0 ]
        then
                mark_b=1
                b=$(( b / i ))
        fi
        if [ $rest_c -eq 0 ]
        then
                mark_c=1
                c=$(( c / i ))
        fi
        if [ $mark_a -eq 1 ] && [ $mark_b -eq 1 ] && [ $mark_c -eq 1 ]
        then
                p=$(( p * i ))
        fi
        if [ $mark_a -ne 1 ]
        then
                i=$(( i+1 ))
        fi
done
echo $p
exit 1

