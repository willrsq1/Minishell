# Minishell
![image](https://github.com/willrsq1/Minishell/assets/117649637/cae75dd9-fbc6-4851-bf0f-fd0ac991efb3)

This project teaches us how to create a shell.

It is also the first 42 project that is requiered to be done as a group.



The requiered part of this program is to handle the basics of a shell:

    - Commands execution
    - Various built-ins
    - Input parsing with and without quotes
    - Input && output redirections
    - Environment management
    - Signal handling

We learn how to use different system functions:

    - Fork, Pipe, Execve
    - Waitpid, Dup2
    - Signal
    - Chdir, pwd


The program is about parsing and execution.
Taking a char* as input, we create an array of tokens, and execute it depending on its contents.
Memory allocation and leaks handling was all done with our own C Garbage Collector (featured on my github).
The same system was used to handle file descriptors, allowing us to open as many redirections/use as many pipes as we want.

It was really fun to do ! But so many small options made it tedious in the end.

We also did the bonuses. It was two different parts: wildcards and handling more complex commands.

Wildcards were easy: some parsing/memory manipulation while using opendir.

Handling "&&", "||" and parenthesis was more challenging, but it ended up going great.

Feel free to try it; some additional options are available and can be enabled, just launch "minishell --help".
Most of them show how the input is treated: what happens inside the program.

![image](https://github.com/willrsq1/Minishell/assets/117649637/e54093c2-2eb3-414c-9e48-6d8b9ce515de)

