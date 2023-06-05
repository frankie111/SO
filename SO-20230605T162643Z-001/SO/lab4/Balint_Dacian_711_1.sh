
#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Not enough parameters"
  exit 1
fi
#cat $1

ctags --fields=+n -x --c-kinds=f $1 > tags
#cat tags
functions=( $(cut -d " " -f 1 tags) )

for function_name in "${functions[@]}"
do
    count=0
    count=$(awk "/\y$function_name(\y)/ && !/int $function_name(\y)/" "$1" | wc -l)
    echo "$function_name: $count"
done
