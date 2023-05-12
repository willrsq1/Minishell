
### COMPILATION ###
CC      = gcc
FLAGS  = -Wall -Wextra -Werror -I -L/usr/local/lib -g3 #-fsanitize=address

### EXECUTABLE ###
NAME   = minishell

### INCLUDES ###
OBJ_PATH  = objs
HEADER = includes
SRC_PATH  = sources

### SOURCE FILES ###
SOURCES =     		main.c \
					1_init/A_tab_creation.c \
					1_init/B_quote_handling.c \
					1_init/C_syntax_check.c \
					2_execve/A_execution.c \
					2_execve/B_pipex.c \
					2_execve/C_pipex_utils.c \
					2_execve/D_command_access.c \
					3_redirections/A_dispatch_redi.c \
					3_redirections/B_infile.c \
					3_redirections/C_outfile.c \
					3_redirections/D_heredoc.c \
					4_builtins/exit_status.c \
					4_builtins/exit.c \
					7_libft_minishell/A_ft_split_minishell.c \
					7_libft_minishell/B_ft_split_minishell_utils.c \
					7_libft_minishell/C_functs.c \
					7_libft_minishell/D_functs_2.c \
					7_libft_minishell/E_functs_3.c \
					7_libft_minishell/F_ft_split.c \
					7_libft_minishell/G_atoi_redi.c \
					7_libft_minishell/gnl/get_next_line.c \
					7_libft_minishell/gnl/get_next_line_utils.c \
					8_utils/A_free_exit.c \
					8_utils/B_malloc.c \
					8_utils/C_open_functs.c \
					8_utils/D_syntax_check_utils.c \
					8_utils/E_print_functions.c \
					9_bonus/A_operands.c \
					9_bonus/B_operands_utils.c \
					9_bonus/C_wildcard.c \
					9_bonus/D_wildcard_utils.c \
					9_bonus/E_wildcard_tab_creation.c \

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
