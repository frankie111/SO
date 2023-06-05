#!/bin/bash

# Überprüfen der Eingabe
if [ $# -eq 0 ]; then
  echo "Bitte geben Sie den Dateinamen als Argument an."
  exit 1
fi

# Überprüfen, ob die Datei existiert
if [ ! -f "$1" ]; then
  echo "Die angegebene Datei existiert nicht."
  exit 1
fi

caesar_encrypt() {
  local input=$1

  # Read the content of the file
  local content=$(cat "$input")

  # Encrypt the content using Caesar cipher
  local encrypted=$(echo "$content" | tr 'a-zA-Z' 'f-za-eF-ZA-E')

  # Overwrite the file with the encrypted content
  echo "$encrypted" > "$input"
}

# Encrypt file using Caesar cipher
caesar_encrypt "$1"
