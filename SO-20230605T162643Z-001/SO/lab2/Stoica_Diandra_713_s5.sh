#!/bin/bash

if [ $# -lt 1 ]
then
echo "not enough parameters"
exit 1
fi

nume_fisier=$1

sed -E 's/\b[[:alnum:]]*[0-9][[:alnum:]]*\b//g' $1

