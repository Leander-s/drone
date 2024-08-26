wc -l $(find -type f ! -path "*include*" ! -path "*build/*" ! -path "*debug/*" ! -path "*pico-sdk*" ! -path "*SDL*" \( -name "*.h" -o -name "*.c" \))
