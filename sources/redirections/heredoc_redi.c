/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 15:35:05 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 14:19:21 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd);

int	ft_dup_heredoc_pipex(char **tab, int i, t_shell *shell)
{
	int	y;

	y = 0;
	while (tab[i][y] && (tab[i][y] >= '0' && tab[i][y] <= '9'))
		y++;
	if (ft_strcmp(&tab[i][y], "<<") == 0)
	{
		ft_fd_redi_heredoc(shell, i, \
			ft_atoi_redi(tab[i], shell, tab[i + 1], 4));
		return (i - 1);
	}
	return (i);
}

void	ft_fd_redi_heredoc(t_shell *shell, int i, int file_fd)
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
		return (heredoc_dup_error(shell, tab, i, file_fd));
	ft_free_close_perror(shell, NULL, 0);
}

static void	heredoc_dup_error(t_shell *shell, char **tab, int i, int file_fd)
{
	int	fd;

	fd = ft_heredoc(ft_strcat(tab[i + 1], "\n", shell), shell);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	ft_remove_redi_ligns(shell, i);
	if (file_fd > 1023)
	{
		shell->bad_open = 1;
		write(2, "Minishell: ", 12);
		ft_putnbr_error(file_fd);
		write(2, ": Bad file descriptor\n", 23);
		return ;
	}
	if (shell->bad_open == 1)
		return ;
	if (dup2(fd, file_fd) == -1)
	{
		ft_add_tbc_list(fd, shell);
		ft_add_tbc_list(file_fd, shell);
		ft_free_close_perror(shell, "dup 2 heredoc", 1);
	}
	return ;
}
