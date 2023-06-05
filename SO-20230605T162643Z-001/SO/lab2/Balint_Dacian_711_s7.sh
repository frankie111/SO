#!/bin/bash

if [ $# -ge 1 ]
then 
sed -E "y/aeiouAEIOU/AEIOUaeiou/" $1
else
echo "Not enough parameters"
exit 1
fi
