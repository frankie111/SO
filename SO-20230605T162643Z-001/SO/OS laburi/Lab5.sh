#!/bin/bash

if [[ $# -lt 2 ]]; then
echo "At least 2 parameters needed"
exit 0
fi

nrLinesInFile=0
averageWordsPerFile=0
nrWordsInFile=0
nrWordsOverall=0
nrFilesOverall=0
averageWordsOverall=0
aux=0

for datei in "$@"; do

nrLinesInFile=$(awk 'END{print NR}' "$datei")
nrWordsInFile=$(awk '{num+=NF} END{print num+0}' "$datei")
averageWordsPerFile=$(($nrWordsInFile / $nrLinesInFile))

echo -e "$datei $averageWordsPerFile"
echo -e ""
nrWordsOverall=$(($nrWordsOverall + $averageWordsPerFile))
nrFilesOverall=$(($nrFilesOverall + 1))
done

averageWordsOverall=$(($nrWordsOverall / $nrFilesOverall))
echo -e "$averageWordsOverall"
