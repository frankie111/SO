#!/bin/bash

msg="n= "
echo -n $msg
read n

if [ $n -eq 0 ] || [ $n -eq 1 ] || [ $n -lt 0 ]
        then echo "Not prime"
        exit 1
fi

for (( i=2; $i<=$n/2; i++ ))
do
        rest=$(( n%i ))
        if [ $rest -eq  0 ]
        then
                echo "Not prime"
                exit 1
        fi
done

echo "Prime"
exit 1