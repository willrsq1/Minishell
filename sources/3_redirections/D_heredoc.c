/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_heredoc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:35:05 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 15:17:54 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_get_heredocs(t_shell *shell)
{
	int		i;
	char	**tab;

	tab = shell->tab;
	shell->bad_open = 0;
	i = -1;
	while (tab[++i])
	{
		if (ft_strcmp(tab[i], "<<") == 0 && \
			shell->is_quoted[i][0] == 0 && shell->is_quoted[i][1] == 0)
		{
			shell->heredoc = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), shell);
			shell->infile = shell->heredoc;
			i = i + 1;
		}
		else if (!arg_is_unquoted(tab[i], shell->is_quoted[i]) && \
			ft_find_redi_with_fd(tab[i], 0))
			i = ft_dup_heredoc_pipex(tab, i, shell);
	}
}

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
			write(2, "\nMinishell: warning: here-document delimited", 45);
			write(2, " by end-of-file (wanted `", 26);
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

int	ft_dup_heredoc_pipex(char **tab, int i, t_shell *shell)
{
	int	y;

	y = 0;
	while (tab[i][y] && (tab[i][y] >= '0' && tab[i][y] <= '9'))
		y++;
	if (ft_strcmp(&tab[i][y], "<<") == 0)
	{
		heredoc_dup_error(shell, tab, i, \
			ft_atoi_redi(tab[i], shell, tab[i + 1], 4));
		return (i - 1);
	}
	return (i);
}

void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd)
{
	int	fd;

	fd = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), shell);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	if (file_fd > 1023)
	{
		shell->bad_open = 1;
		write(2, "Minishell: ", 12);
		ft_putnbr_error(file_fd);
		write(2, ": Bad file descriptor\n", 23);
		ft_remove_redi_ligns(shell, i);
		return ;
	}
	write(2, "Minishell: Unallowed option: ", 30);
	write(2, tab[i], ft_strlen(tab[i]) - 2);
	write(2, ": No heredoc redirections are allowed.\n", 40);
	ft_remove_redi_ligns(shell, i);
	return ;
}
