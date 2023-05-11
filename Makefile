
### COMPILATION ###
CC      = gcc
FLAGS  = -Wall -Wextra -Werror -g -I -L/usr/local/lib -g3 #-fsanitize=address

### EXECUTABLE ###
NAME   = minishell

### INCLUDES ###
OBJ_PATH  = objs
HEADER = includes
SRC_PATH  = sources

### SOURCE FILES ###
SOURCES =     		main.c \
					bonus/operands_utils.c \
					bonus/operands.c \
					bonus/wildcard_tab_creation.c \
					bonus/wildcard_utils.c \
					bonus/wildcard.c \
					builtins/exit_status.c \
					builtins/exit.c \
					execve/execution.c \
					execve/command_access.c \
					execve/path_access.c \
					execve/pipex_heredoc.c \
					execve/pipex_utils.c \
					execve/pipex.c \
					init/pipes_syntax_check.c \
					init/quote_handling.c \
					init/syntax_check.c \
					init/tab_creation.c \
					libft_minishell/atoi_redi.c \
					libft_minishell/ft_split_minishell_utils.c \
					libft_minishell/ft_split_minishell.c \
					libft_minishell/ft_split.c \
					libft_minishell/functs.c \
					libft_minishell/functs_2.c \
					libft_minishell/functs_3.c \
					libft_minishell/gnl/get_next_line.c \
					libft_minishell/gnl/get_next_line_utils.c \
					redirections/dispatch_redi.c \
					redirections/heredoc.c \
					redirections/infile.c \
					redirections/outfile.c \
					utils/free_exit.c \
					utils/open_close.c \
					utils/malloc.c \
					utils/print_functions.c \


### OBJECTS ###

SRCS = $(addprefix $(SRC_PATH)/,$(SOURCES))
OBJS = $(addprefix $(OBJ_PATH)/,$(SOURCES:.c=.o))


### COLORS ###
NOC         = \033[0m
BOLD        = \033[1m
UNDERLINE   = \033[4m
BLACK       = \033[1;30m
RED         = \033[1;31m
GREEN       = \033[1;32m
YELLOW      = \033[1;33m
BLUE        = \033[1;34m
VIOLET      = \033[1;35m
CYAN        = \033[1;36m
WHITE       = \033[1;37m

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^ -lreadline
	@echo "$(GREEN)Project successfully compiled"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HEADER)/*.h Makefile
	@$(CC) $(FLAGS) -c -o $@ $< 
	@echo "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"

clean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -f $(OBJ_PATH)/*.o
	@rm -f $(OBJ_PATH)/*/*.o
	@rm -f $(OBJ_PATH)/*/*/*.o

fclean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -f $(OBJ_PATH)/*.o
	@rm -f $(OBJ_PATH)/*/*.o
	@rm -f $(OBJ_PATH)/*/*/*.o
	@rm -f $(NAME)

norminette:
	@norminette

re: fclean
	@$(MAKE) all

leaks:    all
		echo "{" > valgrind_ignore_leaks.txt
		echo "    leak readline" >> valgrind_ignore_leaks.txt
		echo "    Memcheck:Leak" >> valgrind_ignore_leaks.txt
		echo "    ..." >> valgrind_ignore_leaks.txt
		echo "    fun:readline" >> valgrind_ignore_leaks.txt
		echo "}" >> valgrind_ignore_leaks.txt
		echo "{" >> valgrind_ignore_leaks.txt
		echo "    leak add_history" >> valgrind_ignore_leaks.txt
		echo "    Memcheck:Leak" >> valgrind_ignore_leaks.txt
		echo "    ..." >> valgrind_ignore_leaks.txt
		echo "    fun:add_history" >> valgrind_ignore_leaks.txt
		echo "}" >> valgrind_ignore_leaks.txt
		valgrind --suppressions=valgrind_ignore_leaks.txt --leak-check=full\
			--show-leak-kinds=all --track-origins=yes --verbose --track-fds=yes\
			--show-mismatched-frees=yes --read-var-info=yes ./${NAME}

.PHONY: leaks re fclean clean norminette bonus