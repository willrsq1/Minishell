/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_shell.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 20:56:09 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/22 20:56:46 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_MINISHELL_H
# define STRUCTS_MINISHELL_H

# define COMMAND 127
# define OR_OPERAND 401
# define AND_OPERAND 402
# define SEMICOLON 403

typedef struct s_shell			t_shell;
typedef struct s_pipex			t_pipex;
typedef struct s_to_be_freed	t_to_be_freed;
typedef struct s_to_be_closed	t_to_be_closed;
typedef struct s_init			t_init;
typedef struct s_split			t_split;
typedef struct s_wildc			t_wildc;

typedef struct s_shell
{
	int				nb_of_double_quotes;
	char			**tab;
	t_pipex			*pipex;
	int				infile;
	int				outfile;
	int				heredoc;
	int				*pipe_heredoc;
	int				**is_quoted;
	char			*buff;
	char			quote;
	char			*no_pipes_cmd;
	t_to_be_freed	*to_be_freed_list;
	t_to_be_closed	*tbc_list;
	t_split			*split;
	int				exit;
	int				id_pipe;
	int				i;
	int				exit_status;
}		t_shell;

typedef struct s_pipex
{
	char		**paths;
	char		***commands;
	char		*cmd;
	int			nb_cmds;
	int			**pipe;
	int			*forks_id;
	char		**envp;
	char		**argv;
	int			***is_quoted;
	int			**fds;
	int			*heredoc_fds;
	t_shell		*shell;
}	t_pipex;

typedef struct s_to_be_freed
{
	void			*to_be_freed;
	t_to_be_freed	*next;
}		t_to_be_freed;

typedef struct s_to_be_closed
{
	int				fd;
	t_to_be_closed	*next;
}		t_to_be_closed;

typedef struct s_init
{
	char		**tab;
	int			*mode;
	char		***args;
	int			***is_quoted;
	int			pipes_number;
	t_shell		*shell;
}		t_init;

typedef struct s_split
{
	char		c;
	char		*s;
	int			lignes;
	int			redi_fd;
	int			w;
	int			j;
	int			len;
}		t_split;

typedef struct s_wildc
{
	char		*file;
	t_wildc		*next;
}		t_wildc;

#endif