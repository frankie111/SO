x=$1
y=$2
z=$3
zp=$((z*z))
yp=$((y*y))
xp=$((x*x))
if [ $# -lt 3 ];then
	echo "Sie mussen 3 Parameter eigeben!"
elif [ $((xp+yp)) == $zp ]; then
	echo "Ja"
elif [ $((zp+xp)) == $yp ]; then
	echo "Ja"
elif [ $((zp+yp)) == $xp ]; then
	echo "Ja" 
else
	echo "Nein"
fi
