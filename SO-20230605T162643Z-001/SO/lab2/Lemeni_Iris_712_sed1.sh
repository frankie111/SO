#!/bin/bash

# Überprüft, ob beide Argumente angegeben wurden
if [ $# -lt 2 ]; then
  echo "Usage: $0 <file> <word1> [<word2> ...]"
  exit 1
fi

# Verschieben wir die Argumente so, dass $1 der Dateiname ist und der Rest die zu löschenden Wörter sind
file="$1"
shift

# Loop über die verbleibenden Argumente (d. h. die zu löschenden Wörter) und ersetze sie durch nichts
for word in "$@"; do
  sed -i "s/\b${word}\b//g" "$file"
done


