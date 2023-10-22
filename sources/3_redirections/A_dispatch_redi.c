#include "../../includes/minishell.h"

static int	ft_redirection_with_fd(char *arg, int i, t_shell *shell);
static void	ft_heredoc_was_found(t_shell *shell, int i);

/*	Gets all the redirections, one by one, and executes them.
	Removes the redirections tokens from the input data.
	If error, ends the execution.*/

void	ft_get_redi(t_shell *shell)
{
	int		i;
	char	**tab;
	int		**is_quoted;

	tab = shell->tab;
	is_quoted = shell->is_quoted;
	i = 0;
	while (shell->to_be_freed_list && tab[i])
	{
		if (ft_redirection_with_fd(tab[i], i, shell) == OK)
			continue ;
		else if (ft_strcmp_unquoted(tab[i], "<<", is_quoted[i]) == OK)
			ft_heredoc_was_found(shell, i);
		else if (ft_strcmp_unquoted(tab[i], "<", is_quoted[i]) == OK)
			ft_infile(shell, i);
		else if (ft_strcmp_unquoted(tab[i], ">>", is_quoted[i]) == OK)
			ft_outfile(shell, i, APPEND);
		else if (ft_strcmp_unquoted(tab[i], ">", is_quoted[i]) == OK)
			ft_outfile(shell, i, TRUNC);
		else
			i++;
	}
}

/*	Checks for and executes "XXX>XXX" redirections.
	Could be 2>err_file: this redirects stderr onto err_file.
	Could be 7>fd_7, ect.*/

static int	ft_redirection_with_fd(char *arg, int i, t_shell *shell)
{
	int		y;
	char	*next;

	if (arg_is_unquoted(arg, shell->is_quoted[i]) == ERROR || \
		ft_find_redi_with_fd(arg, 0) == OK)
		return (FAIL);
	y = 0;
	next = shell->tab[i + 1];
	while (arg[y] && (arg[y] >= '0' && arg[y] <= '9'))
		y++;
	if (ft_strcmp(&arg[y], "<<") == OK)
		heredoc_dup_error(shell, shell->tab, i, ft_atoi(arg, shell, next, 1));
	else if (ft_strcmp(&arg[y], "<") == OK)
		ft_infile_with_fd(shell, i, ft_atoi(arg, shell, next, RDONLY));
	else if (ft_strcmp(&arg[y], ">>") == OK)
		ft_outfile_with_fd(shell, i, ft_atoi(arg, shell, next, APPEND), APPEND);
	else if (ft_strcmp(&arg[y], ">") == OK)
		ft_outfile_with_fd(shell, i, ft_atoi(arg, shell, next, TRUNC), TRUNC);
	return (OK);
}

/*	When a heredoc is found here, its creation/writing to has already been done.
	If pipex exists, the input contained pipes. The "HEREDOC_IN_PIPES" value
		will make the program get the heredoc's fd in p->fds[i].
	If the input is without pipes, we simply get the fd. */

static void	ft_heredoc_was_found(t_shell *shell, int i)
{
	if (shell->pipex)
		shell->infile = HEREDOC_IN_PIPES;
	else
		shell->infile = shell->heredoc;
	ft_remove_two_tokens(shell, i);
}

/*	Removes tab[i] and tab[i + 1] from the data.
	They are not needed anymore because their redirection has happened.
	We can take them out of the input now. */

void	ft_remove_two_tokens(t_shell *shell, int i)
{
	while (shell->tab[i + 2])
	{
		shell->is_quoted[i] = shell->is_quoted[i + 2];
		shell->tab[i] = ft_strdup(shell->tab[i + 2], shell);
		i++;
	}
	shell->tab[i] = NULL;
	shell->is_quoted[i] = NULL;
	shell->is_quoted[i + 1] = NULL;
	shell->tab[i + 1] = NULL;
}
