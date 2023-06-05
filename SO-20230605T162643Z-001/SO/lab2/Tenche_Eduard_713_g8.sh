if [ $# -lt 1 ]
then
	echo "Kein Datei gegeben"
	exit 1
else
	sed 's/\([^:]*\):\([^:]*\):\([^:]*\):\(.*\)/\1:\2:\1:\4/' "$1"
fi
