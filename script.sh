#!/bin/bash

# Output file
OUTPUT_FILE="minishell_sources.txt"

# Clear or create the output file
echo "" > "$OUTPUT_FILE"

cat "include/struct.h" >> "$OUTPUT_FILE"

# Loop through all .c files in /source (excluding 'z*')
for file in source/*.c; do
    filename=$(basename "$file")
    if [[ $filename != z* ]]; then
        echo -e "\n\n## File: $file" >> "$OUTPUT_FILE"
        cat "$file" >> "$OUTPUT_FILE"
    fi
done

echo "✅ All .c files (except z*) combined into $OUTPUT_FILE"