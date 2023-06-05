#!/bin/bash

directory_count=0
file_count=0

files=$(find "$1" -type f)
for file in $files; do
  file_count=$((file_count+1))
done

directories=$(find "$1" -type d)
for directory in $directories; do
  directory_count=$((directory_count+1))
done

echo "Gesamtanzahl Dateien: $file_count"
echo "Gesamtanzahl Verzeichnisse: $directory_count"