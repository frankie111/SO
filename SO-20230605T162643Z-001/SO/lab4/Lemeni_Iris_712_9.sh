#!/bin/bash

# Überprüfen, ob alle Parameter gegeben wurden
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <folder> <function_name> <replacement_value>"
  exit 1
fi

# Ordner, in dem sich die .c-Dateien befinden
folder="$1"

# Name der Funktion, deren Parameter ersetzt werden soll
function_name="$2"

# Der Wert, durch den der Parameter ersetzt werden soll
replacement_value="$3"

# Anzahl der vorgenommenen Ersetzungen
num_replacements=0

# Suche nach .c-Dateien im angegebenen Ordner und Bearbeitung jeder Datei
for file in "$folder"/*.c; do
  # Ersetze das erste Vorkommen von function_name(arg) durch function_name(replacement_value)
  sed -i "s/$function_name([^)]*)/$function_name($replacement_value)/" "$file"

  #Überprüfen, ob eine Ersetzung vorgenommen wurde und erhöhen Sie den Zähler
  if [ "$?" -eq 0 ]; then
    num_replacements=$((num_replacements+1))
  fi
done

echo "Number of replacements: $num_replacements"
