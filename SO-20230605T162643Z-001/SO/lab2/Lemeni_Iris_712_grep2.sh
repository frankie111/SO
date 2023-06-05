#!/bin/bash

# Überprüft ob, beide Argumente gegeben wurden
if [ $# -ne 2 ]; then
  echo "Usage: $0 <directory> <word>"
  exit 1
fi

# Verwenden wir grep, um Dateien zu finden, die das Wort enthalten
files=$(grep -l "$2" $1/*)

# Gibt die Namen der übereinstimmenden Dateien aus, einen pro Zeile
for file in $files; do
  echo $file
done

# Zählen wir die Anzahl der übereinstimmenden Dateien und drucken wir die Zählung aus
count=$(echo "$files" | wc -l)
echo "Number of files containing the word \"$2\": $count"
