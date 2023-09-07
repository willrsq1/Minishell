
#ifndef STRUCTS_MINISHELL_H
# define STRUCTS_MINISHELL_H

# define HEREDOC_IN_PIPES -2
# define FAIL -1
# define NO_REDI -1
# define NO_HEREDOC -1
# define OK 0
# define NO_VALUE 0
# define CHILD 0
# define ERROR 1
# define SYNTAX_ERROR 2
# define DIRECTORY_ERROR 126
# define PERMISSION_ERROR 126
# define COMMAND_ERROR 127
# define SIGINT_EXITVALUE 130
# define SEGFAULT_EXITVALUE 139
# define OR_OPERAND 401
# define AND_OPERAND 402
# define SEMICOLON 403
# define RDONLY 801
# define TRUNC 802
# define APPEND 803
# define IS_QUOTED_END -42

typedef struct s_shell			t_shell;
typedef struct s_pipex			t_pipex;
typedef struct s_to_be_freed	t_to_be_freed;
typedef struct s_to_be_closed	t_to_be_closed;
typedef struct s_split			t_split;
typedef struct s_wildc			t_wildc;

typedef struct s_shell
{
	int				no_exit;
	char			**envp;
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
	int				show_tokens;
	int				show_tokens_operands;
	int				show_exit_status;
	int				exit_after_first_input;
	int				enable_semicolons;
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