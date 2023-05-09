#!/bin/bash

echo "Error file :" > err.txt
echo "Results :" > results.txt
sleep 1

echo "Starting loops."

# Loop 10 times
for (( i=1; i<=10; i++ ))
do
  # Step 1
  valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell 2>> err.txt 1>>err.txt &

  # Wait for minishell to start up
  sleep 1

  # Step 2
  cat /dev/urandom | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell 2>>err.txt 1>>err.txt &

  # Step 3
  echo "Please wait 30 seconds."
  sleep 30
  kill -9 $(ps aux | grep /usr/bin/valgrind | awk '{print $2}')
  echo "Loop $i DONE"
done
  # Step 4
  grep -an "signal" err.txt >> results.txt || echo "No Segfault !" >> results.txt
  grep -an "0x1" err.txt | grep -av "main" >> results.txt || echo "No leaks !" >> results.txt
  grep -an "open" err.txt | grep -av "2 open" | grep -av "3 open" >> results.txt || echo "No unclosed files !" >> results.txt
 # grep -an "Invalid" err.txt >>results.txt || echo "No Invalid read/write !" >> results.txt
  grep -an "Invalid" err.txt | awk '/Invalid/ { split($0, a, ":"); printf "%s:", a[1]; for (i=1; i<=NF; i++) { if ($i ~ /Invalid/) { for (j=i; j<=i+4; j++) if ($j !~ /^Minishell/) printf "%s ", $j } } printf "\n" }' >> results.txt || echo "No Invalid read/write !" >> results.txt
echo -e "\n\n\n"
echo "input 'cat results.txt' to see your results."
echo "The line_nb of an error is at the beginning of a line."
echo "Use 'tail -n +LIGN_NB err.txt | head -n 20' to see the full error."

