#include "../../includes/minishell.h"

/*	Gets all the heredocs of the input. Returns the read end of the last pipe.
	Is used for both execution with and without pipes.
	Is used in a loop by the execution with pipes, to get all heredocs before
		forking.*/

int	ft_get_heredocs(t_shell *shell)
{
	int		i;
	char	**tab;

	tab = shell->tab;
	i = -1;
	while (tab[++i])
	{
		if (ft_strcmp_unquoted(tab[i], "<<", shell->is_quoted[i]) == OK)
		{
			shell->heredoc = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), \
				shell);
			shell->infile = shell->heredoc;
			if (shell->infile == SIGINT_EXITVALUE)
				return (SIGINT_EXITVALUE);
			i = i + 1;
		}
		else if (arg_is_unquoted(tab[i], shell->is_quoted[i]) == OK && \
			ft_find_redi_with_fd(tab[i], 0))
			i = ft_dup_heredoc_pipex(tab, i, shell);
	}
	return (0);
}

/*	Reads from STDIN until delimiter with '\n' is found. */

int	ft_heredoc(char *delimiter, t_shell *shell)
{
	char	*buffer;

	shell->pipe_heredoc = ft_calloc(sizeof(int) * 2, shell);
	ft_pipe(shell->pipe_heredoc, shell);
	while (write(1, "heredoc > ", 11))
	{
		buffer = get_next_line(0);
		if (!buffer)
		{
			if (write(2, "\n", 2) && g_exit_code == SIGINT_EXITVALUE)
				return (SIGINT_EXITVALUE);
			write(2, "Minishell: warning: here-document delimited by ", 47);
			write(2, "end-of-file (wanted `", 22);
			delimiter[ft_strlen(delimiter) - 1] = '\0';
			write(2, delimiter, ft_strlen(delimiter));
			write(2, "')\n", 4);
			break ;
		}
		if (!ft_strcmp(buffer, delimiter))
			break ;
		write(shell->pipe_heredoc[1], buffer, ft_strlen(buffer));
		free(buffer);
	}
	close(shell->pipe_heredoc[1]);
	return (free(buffer), shell->pipe_heredoc[0]);
}

/*	Opens a heredoc and dups it onto the file specified. */

int	ft_dup_heredoc_pipex(char **tab, int i, t_shell *shell)
{
	int	y;

	y = 0;
	while (tab[i][y] && (tab[i][y] >= '0' && tab[i][y] <= '9'))
		y++;
	if (ft_strcmp(&tab[i][y], "<<") == OK)
	{
		heredoc_dup_error(shell, tab, i, \
			ft_atoi(tab[i], shell, tab[i + 1], 4));
		return (i - 1);
	}
	return (i);
}

/*	If the user inputs it, refuse to execute it. Follow through though. */

void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd)
{
	int	fd;

	fd = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), shell);
	if (fd == SIGINT_EXITVALUE)
		return ;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	if (file_fd > 1023)
	{
		write(2, "Minishell: ", 12);
		ft_putnbr_error(file_fd);
		write(2, ": Bad file descriptor\n", 23);
		ft_remove_two_tokens(shell, i);
		ft_end_program(shell, OK, EXIT_FAILURE);
	}
	write(2, "Minishell: Unallowed option: ", 30);
	write(2, tab[i], ft_strlen(tab[i]) - 2);
	write(2, ": No heredoc redirections are allowed.\n", 40);
	ft_remove_two_tokens(shell, i);
	return ;
}
