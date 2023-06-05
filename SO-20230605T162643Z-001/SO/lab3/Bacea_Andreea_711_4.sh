Wie kann man das Ergebnis alphabetisch sortieren bei der Standardausgabe angezeigt in shell skript#!/bin/bash
#mit echo kann man text auf dem bildschirm sehen

#wir uberprufen ob es mindestens zwei argumente ubergeben wurden
if [ $# -eq 0 ]; then
        echo "Tuen sie die Dataien"
        exit 1
fi

#wir speichern alle Datein in $@ 
for file in "$@";do
	awk '
	{
		#NF=anzhal Felder
		for(i=2;i<=NF;i++){
			if($i == $(i-1)) {
				if($i !=vor_wort){
				#in den vor_wort speichert man den vorigen Wort in der zeile zu speichern, se verifica ca e diferit ca sa nu l fi afisat 
				#deja pe ecran pe aceasi linie 
					#NR  este linia curenta
					print NR, $i
					vor_wort=$i
				}
			}
			else{
				vor_wort=""
			}
		}
	}
	' "$file" #end of file 
done
