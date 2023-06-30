/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 06:24:48 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/20 06:24:48 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "structs_minishell.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h> //waitpid
# include <sys/wait.h> //waitpid
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <limits.h>

/*		MAIN.C */
extern int	g_exit_code;

// int		main(int argc, char **argv, char **envp);
void	ft_signal(t_shell *shell);

/* ------------------------------------------------------------------------- */
/* -------------------------- 1_INIT --------------------------------------- */
/* ------------------------------------------------------------------------- */

/* 		A_TAB_CREATION.C */

int		tab_creation(t_shell *shell, char *buff);

/*		B_QUOTE_HANDLING.C */

char	**quotes_management(t_shell *shell, char **tab);
char	ft_which_quote(char *str);

/*		C_SYNTAX_CHECK.C */

int		ft_syntax_error(char **tab, t_shell *shell);

/*		D_SYNTAX_CHECK_BONUS.C */

int		ft_check_parenthesis_tokens(char **tab, t_shell *shell);
int		ft_check_parenthesis_chars(char **tab, int **iq, int quote, int y);

/*		E_SYNTAX_CHECK_UTILS.C */

int		ft_is_token_meta(char *arg, int *is_quoted);
int		ft_is_token_redi(char *arg, int *is_quoted);
int		ft_is_token_redi_not_heredoc(char *arg, int *is_quoted);
int		ft_is_token_operand(char *arg, int *is_quoted);

/* ------------------------------------------------------------------------- */
/* -------------------------- 2_EXECVE ------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_EXECUTION.C */

void	ft_do_the_execve_thing(t_shell *shell, char **envp);

/*		B_EXEC_NO_PIPES.C */

void	exec_no_pipes(t_shell *shell, char **envp);
void	ft_dup2_exec_no_pipes(t_shell *shell);

/*		C_EXEC_PIPES.C */

void	ft_pipex(int argc, t_init *init, char **envp);

/*		D_EXEC_PIPES_UTILS.C */

void	ft_pipex_initialisation(t_pipex *p);
void	ft_pipex_big_tab(int pipes_number, t_init *init, t_shell *shell);
void	ft_get_heredocs_pipex(t_pipex *p, int i);
void	ft_close_pipes(int i, t_pipex *p, int x);

/*		E_COMMAND_ACCESS.C */

void	ft_get_envp_paths(t_pipex *p, char **envp);
int		ft_get_cmd(t_pipex *p, int i);

/* ------------------------------------------------------------------------- */
/* -------------------------- 3_REDIRECTIONS ------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_DISPATCH_REDI.C */

void	ft_get_redi(t_shell *shell);
void	ft_remove_two_tokens(t_shell *shell, int i);

/*		B_REDIRECTIONS.C */

void	ft_infile(t_shell *shell, int i);
void	ft_outfile(t_shell *shell, int i, int option);
void	ft_infile_with_fd(t_shell *shell, int i, int file_fd);
void	ft_outfile_with_fd(t_shell *shell, int i, int file_fd, int option);

/*		C_HEREDOC.C */

void	ft_get_heredocs(t_shell *shell);
int		ft_heredoc(char *delimiter, t_shell *shell);
void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd);
int		ft_dup_heredoc_pipex(char **tab, int i, t_shell *shell);

/* ------------------------------------------------------------------------- */
/* -------------------------- 4_BUILTINS ----------------------------------- */
/* ------------------------------------------------------------------------- */

/*		EXIT.C */

int		ft_exit(t_shell *shell, char **envp);
int		ft_variables_substitution(t_shell *shell);

/*		A_BUILTINS_DISPATCH.C */

int		ft_builtins_in_child(t_shell *shell, char **tab, char **envp);
int		ft_builtins(t_shell *shell, char **tab, char **envp);

/*		B_ECHO.C */

int		ft_echo(char **tab);

/*		C_CD.C */

int		ft_cd(t_shell *shell, char **tab);
char	*ft_getenv(char *var, t_shell *shell);

/*		D_PWD.C */

int		ft_pwd(void);

/*		E_EXPORT.C */

int		ft_export(t_shell *shell, char **tab, char **envp);

/*		F_UNSET.C */

int		ft_unset(t_shell *shell, char **tab, char **envp);
int		ft_unset_get_envp_lign(char **envp, char *var_name);

/*		G_ENV.C */

int		ft_env(char **envp);

/* ------------------------------------------------------------------------- */
/* -------------------------- 5_VARIABLE_REPLACEMENT ----------------------- */
/* ------------------------------------------------------------------------- */

/*		A_VAR_SEARCH.C */

int		ft_variables_substitution(t_shell *shell);

/*		B_VAR_SUBSTITUTION.C */

void	ft_substitute_var(t_shell *shell, char *var, char *arg, int *is_quoted);

/*		C_EXIT_VALUE_VAR.C */

void	ft_exit_value_sub(t_shell *shell, int z, int y, int len);

/*		D_VAR_UTILS.C */

char	*ft_get_var_name(t_shell *shell, char *arg, int *is_quoted);
int		ft_var_get_envp_lign(char **envp, char *var_name);
int		reading(t_pipex *pipex, int i);
void	ft_remove_one_token(t_shell *shell, int i, int y);
int		ft_var_not_found(char *arg, int *is_quoted);
char	**ft_export_sorted_tab(t_shell *shell, char **envp);

/* ------------------------------------------------------------------------- */
/* -------------------------- 7_LIBFT_MINISHELL ---------------------------- */
/* ------------------------------------------------------------------------- */

/*		GNL/GET_NEXT_LINE.C */

char	*get_next_line(int fd);

/*		GNL/GET_NEXT_LINE_UTILS.C */

char	*ft_strdup_gnl(const char *s);
void	*ft_calloc_gnl(size_t nmemb, size_t size);
void	ft_bzero_gnl(void *s, size_t n);
size_t	ft_strlen_gnl(const char *str);
size_t	ft_strlcpy_gnl(char *dest, const char *src, size_t size);

/* 		A_FT_SPLIT_MINISHELL.C */

char	**ft_split_minishell(char *s, char c, t_shell *shell);

/* 		B_FT_SPLIT_MINISHELL_UTILS.C */

char	*ft_weird_realloc_thing(char *initial, int added_len, t_shell *shell);
int		ft_len_without_quotes(char *s);
int		ft_len_within_quotes(char *s);
int		ft_break_split_loop(char *s, int i);
int		parenthesis(t_split split, t_shell *shell, char **tab, int i);

/*		C_FUNCTS.C */

int		ft_strcmp(char *arg, char *checked);
int		ft_strncmp(char *arg, char *checked, int n);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
char	*ft_strcat(char *s1, char *s2, t_shell *shell);
int		arg_is_unquoted(char *arg, int *is_quoted);

/*		D_FUNCTS_2.C */

void	ft_delete_char(char *str);
void	ft_putnbr_error(int nb);
int		ft_strlen(char *s);
char	*ft_strdup(const char *s, t_shell *shell);

/*		E_FUNCTS_3.C */

int		ft_is_meta_carac(char c);
int		ft_find_redi_with_fd(char *s, int i);
char	*ft_join_tab(char **tab, t_shell *shell);

/*		F_FUNCTS_4.C */

int		ft_strcmp_unquoted(char *arg, char *checked, int *is_quoted);
char	*ft_itoa(int nb, t_shell *shell);
int		ft_atoi(char *str, t_shell *shell, char *file, int mode);

/*		G_FT_SPLIT.C */

char	**ft_split(char *s, char c, t_shell *shell);

/* ------------------------------------------------------------------------- */
/* -------------------------- 8_UTILS -------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_FREE_EXIT.C */

void	ft_free_close_perror(t_shell *shell, char *description, int mode);
void	ft_end_program(t_shell *shell, int mode, int err);
void	ft_clear_memory(t_shell *shell);
void	ft_close_all_fds(t_shell *shell);

/*		B_MALLOC.C */

void	*ft_calloc(size_t nmemb, t_shell *shell);

/*		C_OPEN_FUNCTS.C */

void	ft_add_tbc_list(int fd, t_shell *shell);
int		ft_open(char *file, t_shell *shell, int option);
void	ft_pipe(int *tab, t_shell *shell);

/*		D_PRINT_FUNCTIONS.C */

void	print_tokens(t_shell *shell);
void	print_tokens_operands(char ***operands_tab, int ***operands_is_quoted, \
	int *options);
void	ft_initializing_options(t_shell *shell, int argc, char **argv);

/*		E_MAIN_UTILS.C */

void	ft_create_prompt(t_shell *shell, char **envp, char **argv);
char	**ft_new_envp(char **envp);
void	ft_shlvl(char **envp);

/* ------------------------------------------------------------------------- */
/* -------------------------- 9_BONUS -------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_OPERANDS.C */

int		ft_special_operands(t_shell *shell, char **envp);

/*		E_WILDCARD_TAB_CREATION.C */

char	**ft_wildc_new_init(int count, t_wildc *first, \
	char *dir_path, t_shell *shell);
int		ft_dup_new_tab_wildcard(t_shell *shell, char **tab, char **new_args);

/*		C_WILDCARD.C */

int		ft_wildcards_handling(t_shell *shell, char **tab);

/*		D_WILDCARD_UTILS.C */

void	ft_wildcnew_wildcard(void *file, t_wildc *first, t_shell *shell);
int		ft_check_wildcard_token(char *file, char *new_tok);
char	*next_tok(char *arg, t_shell *shell);
char	*find_wildcard_directory(t_shell *shell, char *arg, char **arg_add);
int		find_wildcard(char *arg, t_shell *shell, int i);

/*		B_OPERANDS_UTILS.C */

int		ft_is_it_operand(char *arg, int *is_quoted);
int		ft_count_operands(t_shell *shell);
int		ft_find_op_lenght(char **tab, t_shell *shell, int i);

#endif