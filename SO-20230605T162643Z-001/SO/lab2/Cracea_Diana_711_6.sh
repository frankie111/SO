#!/bin/bash

# Prüfen, ob sowohl eine Datei als auch ein Ordner angegeben wurden
if [ $# -lt 2 ]; then
  echo "Bitte geben Sie eine Datei und mindestens einen Ordner an."
  exit 1
fi

# Speichern der Datei und der Ordner in separaten Variablen
file="$1"
shift
folders=("$@")

# Löschen aller passenden Dateien in den angegebenen Ordnern 
while read -r line; do
  for folder in "${folders[@]}"; do
    find "$folder" -name "$line" -delete
  done
done < "$file"

