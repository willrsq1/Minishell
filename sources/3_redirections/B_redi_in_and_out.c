/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_infile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 21:08:45 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/31 15:23:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_infile(t_shell *shell, int i)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open(tab[i + 1], shell, RDONLY);
	shell->infile = fd;
	ft_remove_redi_ligns(shell, i);
	if (fd == -1)
		return ;
	return ;
}

void	ft_outfile(t_shell *shell, int i, int option)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open(tab[i + 1], shell, option);
	if (fd == -1)
		return ;
	ft_remove_redi_ligns(shell, i);
	shell->outfile = fd;
	return ;
}

void	ft_fd_redi_infile(t_shell *shell, int i, int file_fd)
{
	int		fd;

	if (file_fd == -1)
		return ;
	fd = ft_open(shell->tab[i + 1], shell, RDONLY);
	ft_remove_redi_ligns(shell, i);
	if (fd == -1)
		return ;
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}

void	ft_fd_redi_outfile(t_shell *shell, int i, int file_fd, int option)
{
	int	fd;

	if (file_fd == -1)
		return ;
	fd = ft_open(shell->tab[i + 1], shell, option);
	ft_remove_redi_ligns(shell, i);
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	if (file_fd == 1)
		shell->outfile = fd;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}
