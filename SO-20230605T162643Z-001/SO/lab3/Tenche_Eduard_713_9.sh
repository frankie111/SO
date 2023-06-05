max=0
maxfile="None"
for file in $@
do
	#count=$(wc -w $file | cut -c1)
	count=$(awk '{num+=NF} END{print num}' $file)
	if [ $count -gt $max ]
	then
		max=$count
		maxfile=$file
	fi
done
echo -e "$maxfile\n$max"
