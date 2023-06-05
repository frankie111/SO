#!/bin/bash
if [ $# -eq 0 ]; then
  echo "Bitte geben Sie den Namen der Textdatei als Argument an."
  exit 1
fi

for file in "$@"; do
  awk 'length > 30 {print FNR, $1, $NF}' "$file"
done
