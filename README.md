# Minishell

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
https://github.com/willrsq1/Garbage_Collector_C
The same system was used to handle file descriptors, allowing us to open as many redirections/use as many pipes as we want.

It was really fun to do ! But so many small options made it tedious in the end.

We also did the bonuses. It was two different parts: wildcards and handling more complex commands.

Wildcards were easy: some parsing/memory manipulation while using opendir.

Handling "&&", "||" and parenthesis was more challenging, but it ended up going great.

Feel free to try it; some additional options are available and can be enabled, just launch "minishell --help".
Most of them show how the input is treated: what happens inside the program.

# 💪 Contributers
[ABDYLI Arbesa](https://github.com/aarbesa)<br/>
&emsp;42intra profile : [aabdyli](https://profile.intra.42.fr/users/aabdyli) <br/>
<br/>
[RUET SUQUET William](https://github.com/willrsq1)
<br/>
&emsp;42intra profile : [wruet-su](https://profile.intra.42.fr/users/wruet-su)
<br/>

# Peer review

![image](https://github.com/willrsq1/Minishell/assets/117649637/0d34df61-a4ea-4680-bb36-7366d73b092d)

# Final Grade

![image](https://github.com/willrsq1/Minishell/assets/117649637/a518225e-5059-40a1-a1da-c83f5f6be1f3)


