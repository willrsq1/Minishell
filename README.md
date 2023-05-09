# Minishell
Ongoing project


How to check for leaks, unclosed opens, invalid reads/writes :

/!\ WARNING /!\ Make a clone of your repository and do the tests in there ! Lots of files can be created / deleted.

Either launch the script "crash_test_minishell", or do it mannually :

1 ) Launch your minishell like this : valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell > err.txt 2>&1

2 ) Copy paste this and hit enter (in your minishell) : cat /dev/urandom | valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell

3 ) input 'exit' (after 30 secs) (or ctrl + C / D if exit fails)

4 ) Copy paste this and hit enter (in your terminal ) : grep -an "signal" err.txt ; grep -an "0x1" err.txt | grep -av "main"  ; grep -an "open" err.txt | grep -av "4 open" ; grep -an "Invalid" err.txt

How to read the output :

If the 4th step returns some text, you have problems !

To see what they are, write this in your terminal to get the chunk of information you need : tail -n +LIGN_NB err.txt | head -n 20
