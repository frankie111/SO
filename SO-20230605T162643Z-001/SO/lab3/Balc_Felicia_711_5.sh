file_name="$1"

awk 'length($0) > 10 {count++} END {print "File name: " FILENAME, ";Number of lines: ", count}' "$file_name" >> "$file_name"

awk 'length($0) > 10 {print substr($0,11)}' "$file_name"
