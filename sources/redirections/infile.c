/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 21:08:45 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 14:42:34 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_redi_dup_infile(char *file, t_shell *shell, int i, int file_fd);

void	ft_infile(t_shell *shell, int i)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open_rdonly(tab[i + 1], shell);
	if (fd == -1)
		return ;
	shell->infile = fd;
	ft_remove_redi_ligns(shell, i);
	return ;
}

void	ft_fd_redi_infile(t_shell *shell, int i, int file_fd)
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
		return (ft_redi_dup_infile(tab[i + 1], shell, i, file_fd));
	ft_free_close_perror(shell, NULL, 0);
}

static void	ft_redi_dup_infile(char *file, t_shell *shell, int i, int file_fd)
{
	int	fd;

	fd = ft_open_rdonly(file, shell);
	ft_remove_redi_ligns(shell, i);
	if (fd == -1)
		return ;
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}
