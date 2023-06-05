#!/bin/bash

# Schleife ueber alle eingegebenen Dateien
for file in "$@"
do
  # Inhalt der Datei in umgekehrter Reihenfolge ausgeben
  awk '{a[NR]=$0} END {for (i=NR;i>=1;i--) print a[i]}' "$file"
done

