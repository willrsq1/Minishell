
### COMPILATION ###
CC      = gcc
FLAGS  = -Wall -Wextra -Werror

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
					1_init/D_syntax_check_bonus.c \
					1_init/E_syntax_check_utils.c \
					2_execve/A_execution.c \
					2_execve/B_exec_no_pipes.c \
					2_execve/C_exec_pipes.c \
					2_execve/D_exec_pipes_utils.c \
					2_execve/E_command_access.c \
					3_redirections/A_dispatch_redi.c \
					3_redirections/B_redi_in_and_out.c \
					3_redirections/C_heredoc.c \
					4_builtins/A_builtins_dispatch.c \
					4_builtins/B_echo.c \
					4_builtins/C_cd.c \
					4_builtins/D_pwd.c \
					4_builtins/E_export.c \
					4_builtins/F_unset.c \
					4_builtins/G_env.c \
					4_builtins/H_exit.c \
					5_variable_replacement/A_var_search.c \
					5_variable_replacement/B_var_substitution.c \
					5_variable_replacement/C_exit_value_var.c \
					5_variable_replacement/D_var_utils.c \
					6_signal/signal.c \
					7_libft_minishell/gnl/get_next_line.c \
					7_libft_minishell/gnl/get_next_line_utils.c \
					7_libft_minishell/A_ft_split_minishell.c \
					7_libft_minishell/B_ft_split_minishell_utils.c \
					7_libft_minishell/C_functs.c \
					7_libft_minishell/D_functs_2.c \
					7_libft_minishell/E_functs_3.c \
					7_libft_minishell/F_functs_4.c \
					7_libft_minishell/G_ft_split.c \
					8_utils/A_free_exit.c \
					8_utils/B_malloc.c \
					8_utils/C_open_functs.c \
					8_utils/D_print_functions.c \
					8_utils/E_main_utils.c \
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
	@echo "$(GREEN)Project successfully compiled\n$(CYAN)\n\nWelcome.\nRun ./minishell --help to learn about the additional commands.\n\n"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(HEADER)/*.h Makefile
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c -o $@ $< 
	@echo "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"

clean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -rf $(OBJ_PATH)

fclean:
	@echo "$(GREEN)Supressing libraries files$(CYAN)"
	@rm -rf $(OBJ_PATH)
	@rm -f $(NAME)

norminette:
	@norminette

re: fclean
	@$(MAKE) all

bonus: all

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
