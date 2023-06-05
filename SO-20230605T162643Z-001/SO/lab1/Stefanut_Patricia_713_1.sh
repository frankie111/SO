#!/bin/bash

# Durchsuche den aktuellen Ordner und Unterordner und liste alle Dateien auf, für die "others" Schreibrechte haben
find . -type f -perm /o+w -print


