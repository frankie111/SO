if [ $# -lt 2 ]; then
	echo "Sie mussen mindestens zwei Parameter eingeben!"
fi
file_name="$1"
shift

for wort in "$@"
do
	sed -i "s/$wort//g" "$file_name"
done
