/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outfile_fd_redi.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 23:56:49 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 13:33:02 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_fd_redi_append(char *file, t_shell *shell, int i, int file_fd);
static void	ft_fd_redi_trunc(char *file, t_shell *shell, int i, int file_fd);

void	ft_fd_redi_outfile_append(t_shell *shell, int i, int file_fd)
{
	char	**tab;

	if (file_fd == -1)
		return ;
	tab = shell->tab;
	if (!tab[i + 1])
		write(2, "mini: syntax error near unexpected token `newline'\n", 52);
	else if (ft_is_meta_carac(tab[i + 1][0]) == 1 && \
		shell->is_quoted[i + 1][0] == 0)
	{
		write(2, "mini: syntax error near unexpected token `", 43);
		write(2, tab[i + 1], ft_strlen(tab[i + 2]));
		write(2, "'\n", 3);
	}
	else
		return (ft_fd_redi_append(tab[i + 1], shell, i + 1, file_fd));
	ft_free_close_perror(shell, NULL, 0);
}

static void	ft_fd_redi_append(char *file, t_shell *shell, int i, int file_fd)
{
	int	fd;

	fd = ft_open_append(file, shell);
	ft_remove_redi_ligns(shell, i - 1);
	if (fd == -1)
		return ;
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}

void	ft_fd_redi_outfile_trunc(t_shell *shell, int i, int file_fd)
{
	char	**tab;

	if (file_fd == -1)
		return ;
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
		return (ft_fd_redi_trunc(tab[i + 1], shell, i, file_fd));
	ft_free_close_perror(shell, NULL, 0);
}

static void	ft_fd_redi_trunc(char *file, t_shell *shell, int i, int file_fd)
{
	int	fd;

	fd = ft_open_trunc(file, shell);
	ft_remove_redi_ligns(shell, i);
	if (fd == -1)
		return ;
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}
