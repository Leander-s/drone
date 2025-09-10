wc -l $(find -type f ! -path "*include*" ! -path "*build/*" ! -path "*debug/*" ! -path "*glad*" ! -path "*cglm*" ! -path "*pico-sdk*" ! -path "*SDL*" \( -name "*.h" -o -name "*.c" \))
