#!/bin/bash
text=$1
shift
if [ $# -gt 1 ];then
for datei in $@
do
if [ -e $datei ];then
sed -E "/$text/d" $datei
fi
done
fi

