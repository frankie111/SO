#!/bin/bash
total_words=0
total_files=0

for file in "$@"; do
  if [ -f "$file" ]; then
    words=$(cat "$file" | awk '{print NF}')
    echo "$file $words"
    total_words=$((total_words + words))
    total_files=$((total_files + 1))
  fi
done

if [ "$total_files" -gt 0 ]; then
  avg_words=$((total_words / total_files))
  echo "Average words per file: $avg_words"
fi

