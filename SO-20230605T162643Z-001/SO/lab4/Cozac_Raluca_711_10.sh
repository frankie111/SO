#!/bin/bash

# Verifică dacă există cel puțin un parametru (numele clientului)
if [ $# -eq 0 ]; then
    echo "Vă rugăm să furnizați cel puțin un nume de client ca parametru"
    exit 1
fi

# Parcurge fiecare client în lista de parametri
for customer in "$@"; do
    # Verifică dacă fișierul clientului există
    if [ ! -f "${customer}" ]; then
        echo "Fișierul clientului ${customer} nu a fost găsit"
        continue
    fi

    # Extrage datele clientului din fișierul corespunzător
    read income < "${customer}"
    expenses=$(sed -n '2p' "${customer}")
    fund=0
    months=0

	for num in $expenses; do
 	 if [ $num -lt $income ]; then
	 months=$(($months + 1))
	 fund=$(($fund + $income - $num))
 	 fi
	done 

    # Adaugă rezultatele la fișierul clientului
    echo "${fund} ${months}" >> "${customer}"
done

