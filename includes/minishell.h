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
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>

/*		MAIN.C */

// int		main(int argc, char **argv, char **envp);

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

/* ------------------------------------------------------------------------- */
/* -------------------------- 2_EXECVE ------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_EXECUTION.C */

void	ft_do_the_execve_thing(t_shell *shell, char **envp);

/*		B_PIPEX.C */

void	ft_pipex(int argc, t_init *init, char **envp);

/*		C_PIPEX_UTILS.C */

void	ft_pipex_initialisation(t_pipex *p);
void	ft_pipex_big_tab(int pipes_number, t_init *init, t_shell *shell);
void	ft_get_heredocs_pipex(t_pipex *p, int i);
void	ft_close_pipes(int i, t_pipex *p);

/*		E_COMMAND_ACCESS.C */

void	ft_get_envp_paths(t_pipex *p, char **envp);
int		ft_get_cmd(t_pipex *p, int i);

/* ------------------------------------------------------------------------- */
/* -------------------------- 3_REDIRECTIONS ------------------------------- */
/* ------------------------------------------------------------------------- */

/*		A_DISPATCH_REDI.C */

void	ft_get_redi(t_shell *shell);
void	ft_remove_redi_ligns(t_shell *shell, int i);

/*		B_REDIRECTIONS.C */

void	ft_infile(t_shell *shell, int i);
void	ft_outfile(t_shell *shell, int i, int option);
void	ft_fd_redi_infile(t_shell *shell, int i, int file_fd);
void	ft_fd_redi_outfile(t_shell *shell, int i, int file_fd, int option);

/*		C_HEREDOC.C */

void	ft_get_heredocs(t_shell *shell);
int		ft_heredoc(char *delimiter, t_shell *shell);
void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd);
int		ft_dup_heredoc_pipex(char **tab, int i, t_shell *shell);

/* ------------------------------------------------------------------------- */
/* -------------------------- 4_BUILTINS ----------------------------------- */
/* ------------------------------------------------------------------------- */

/*		EXIT.C */

void	ft_exit(t_shell *shell);
int		ft_find_exit_status(t_shell *shell);

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
int		ft_should_i_break(char *s, int i);
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
char 	*ft_join_tab(char **tab, t_shell *shell);

/*		F_FT_SPLIT.C */

char	**ft_split(char *s, char c, t_shell *shell);

/*		G_ATOI_REDI.C */

char	*ft_itoa(int nb, t_shell *shell);
int		ft_atoi(char *str, t_shell *shell, char *file, int mode);

/* ------------------------------------------------------------------------- */
/* -------------------------- 8_UTILS -------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		FREE_EXIT.C */

void	ft_free_close_perror(t_shell *shell, char *description, int mode);
void	ft_end_program(t_shell *shell, int mode, int err);
void	ft_free_everything_lol(t_shell *shell);
void	ft_close_everything_lol(t_shell *shell);

/*		MALLOC.C */

void	*ft_calloc(size_t nmemb, t_shell *shell);

/*		OPEN_CLOSE.C */

void	ft_add_tbc_list(int fd, t_shell *shell);
int		ft_open(char *file, t_shell *shell, int option);
void	ft_pipe(int *tab, t_shell *shell);

/*		SYNTAX_CHECK_UTILS.C */

int		ft_is_token_operand(char *arg, int *is_quoted);
int		ft_is_token_meta(char *arg, int *is_quoted);
int		ft_is_token_redi(char *arg, int *is_quoted);

/*		E_PRINT_FUNCTIONS.C */

void	print_tokens(t_shell *shell);
void	print_tokens_operands(char ***operands_tab, int ***operands_is_quoted, \
	int *options);
void	ft_initializing_options(t_shell *shell, int argc, char **argv);


/* ------------------------------------------------------------------------- */
/* -------------------------- 9_BONUS -------------------------------------- */
/* ------------------------------------------------------------------------- */

/*		E_WILDCARD_TAB_CREATION.C */

char	**ft_wildc_new_init(int count, t_wildc *first, \
	char *dir_path, t_shell *shell);
int		ft_dup_new_tab_wildcard(t_shell *shell, char **tab, char **new_args);

/*		C_WILDCARD.C */

int		ft_find_wildcard(t_shell *shell, char **tab);

/*		D_WILDCARD_UTILS.C */

void	ft_wildcnew_wildcard(void *file, t_wildc *first, t_shell *shell);
int		ft_is_invalid_arg_wildcard(char *arg, char *file);
char	*find_wildcard_directory(t_shell *shell, char *arg, char **arg_add);
int		find_wildcard(char *arg, t_shell *shell, int i);

/*		B_OPERANDS_UTILS.C */

int		ft_is_it_operand(char *arg, int *is_quoted);
int		ft_count_operands(t_shell *shell);
int		ft_find_op_lenght(char **tab, t_shell *shell, int i);

/*		A_OPERANDS.C */

int		ft_no_operands(t_shell *shell, char **envp, int w);

#endif