#!/bin/bash

rm err.txt results.txt
sleep 1

# Step 1
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell 2>> err.txt 1>>err.txt &

# Wait for minishell to start up
sleep 1

# Step 2
cat /dev/urandom | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell 2>>err.txt 1>>err.txt &

# Step 3
sleep 10
echo "exit"

# Step 4
grep -an "signal" err.txt >> results.txt || echo "No Segfault !" >> results.txt
grep -an "0x1" err.txt | grep -av "main" >> results.txt || echo "No leaks !" >> results.txt
grep -an "open" err.txt | grep -av "2 open" | grep -av "3 open" >> results.txt || echo "No unclosed files !" >> results.txt
grep -an "Invalid" err.txt >>results.txt || echo "No Invalid read/write !" >> results.txt

echo "input 'cat results.txt' to see your results" 1>&1

# Clean up

