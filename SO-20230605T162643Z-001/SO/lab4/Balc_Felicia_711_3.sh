cp notenSommer.csv notenSommer.csv.bak
cat notenSommer.csv | awk -F ',' '{sum=0; for(i=3; i<=NF; i++) sum+=$i; media=sum/(NF-2); print media " " $0}' | sort -rn | cut -d ' ' -f 2- > notenSommer.csv

cp notenSommer.csv notenSommer.csv.bak
awk -F ',' '{gsub(/ /, ","); sum=0; for(i=3; i<=NF; i++) sum+=$i; media=sum/(NF-2); $2=$2 " " media; $3=""; print}' notenSommer.csv > tmp.csv && mv tmp.csv notenSommer.csv

cp notenSommer.csv notenSommer.csv.bak
sed -i 's/\(\S\+\s\+\S\+\s\+\S\+\s\+\)[^\s]*.*/\1/' notenSommer.csv

cp notenSommer.csv notenSommer.csv.bak
awk -F',| ' 'FNR==NR{a[$1" "$2]=$3; next}{print $1,$2","$3,a[$1" "$2]}' notenSommer.csv studenten.csv > test.csv

cp notenSommer.csv notenSommer.csv.bak
cp test.csv test.csv.bak
awk -F'[, ]' '{sum=0; count=0; for(i=2;i<=NF; i++) {if($i~/^[0-9]/) {sum+=$i; count++;}} avg=sum/count; print $1,$2,avg}' test.csv > tmpfile && mv tmpfile test.csv

sed -i '1,$d' studenten.csv && cat test.csv >> studenten.csv
