if [ $# -lt 1 ]
	then
		echo "Kein Datei gegeben"
		exit 1
fi

for file in $1/*
do
	if [[ "$file" == *.c ]]
		then
		if [ $(grep -c "#include" $file) -gt 2 ]
			then
			echo "$file"
		fi
		
	fi
done
