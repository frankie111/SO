if [ $# -lt 6 ]; then
    echo "Error: 6 arguments are required." >&2
    exit 1
fi
a=$1
b=$2
c=$3
e=$4
f=$5
g=$6

skalarprodukt=$((a*e + b*f + c*g))

if (( $skalarprodukt == 0 )); then
   echo "ja"
else
   echo "nein"
fi

