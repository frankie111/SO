#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Bitte geben Sie den Namen der Textdatei als Argument an."
  exit 1
fi

if [ ! -d "Worterbuch" ]; then
  mkdir "Worterbuch"
fi

for i in {0..9}; do
  touch "Worterbuch/$i"
  while read -r line; do
    for word in $line; do
      if [[ "$word" =~ ^$i ]]; then
        echo "$word" >> "Worterbuch/$i"
      fi
    done
  done < "$1"
  sort -o "Worterbuch/$i"
done

