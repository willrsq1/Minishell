/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 00:31:54 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 14:58:28 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_get_fd_heredoc(t_shell *shell, int i);

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
			ft_get_fd_heredoc(shell, i);
			i = i + 1;
		}
		else if (arg_is_unquoted(tab[i], shell->is_quoted[i]) == 0 && \
			ft_find_redi_with_fd(tab[i], 0) != 0)
			i = ft_dup_heredoc_pipex(tab, i, shell);
	}
}

static void	ft_get_fd_heredoc(t_shell *shell, int i)
{
	char	**tab;

	tab = shell->tab;
	if (!tab[i + 1])
		write(2, "mini: syntax error near unexpected token `newline'\n", 52);
	else if (ft_is_meta_carac(tab[i + 1][0]) == 1 && \
		shell->is_quoted[i + 1][0] == 0)
	{
		write(2, "mini: syntax error near unexpected token `", 43);
		write(2, tab[i + 1], ft_strlen(tab[i + 1]));
		write(2, "'\n", 3);
	}
	else
	{
		shell->heredoc = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), shell);
		shell->infile = shell->heredoc;
		return ;
	}
	shell->bad_open = 1;
	ft_free_close_perror(shell, NULL, 0);
}

int	ft_heredoc(char *delimiter, t_shell *shell)
{
	char	*buffer;

	shell->pipe_heredoc = ft_calloc(sizeof(int) * 2, shell);
	ft_pipe(shell->pipe_heredoc, shell);
	while (write(2, "heredoc > ", 11))
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
