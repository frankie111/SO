#!/bin/bash

if [  $# -ne 1 ]; then
echo "Wrong number of parameters"
fi

cd $1

grep -rl "#define" *.c | sort

