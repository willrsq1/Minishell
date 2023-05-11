/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_outfile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 21:01:45 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 19:31:21 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_outfile_append(t_shell *shell, int i)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open_append(tab[i + 1], shell);
	if (fd == -1)
		return ;
	ft_remove_redi_ligns(shell, i);
	shell->outfile = fd;
	return ;
}

void	ft_outfile_trunc(t_shell *shell, int i)
{
	int		fd;
	char	**tab;

	tab = shell->tab;
	fd = ft_open_trunc(tab[i + 1], shell);
	if (fd == -1)
		return ;
	shell->outfile = fd;
	ft_remove_redi_ligns(shell, i);
	return ;
}

void	ft_fd_redi_outfile_append(t_shell *shell, int i, int file_fd)
{
	int	fd;

	if (file_fd == -1)
		return ;
	fd = ft_open_append(shell->tab[i + 1], shell);
	ft_remove_redi_ligns(shell, i);
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);
	if (file_fd == 1)
		shell->outfile = fd;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}

void	ft_fd_redi_outfile_trunc(t_shell *shell, int i, int file_fd)
{
	int		fd;

	fd = ft_open_trunc(shell->tab[i + 1], shell);
	ft_remove_redi_ligns(shell, i);
	if (dup2(fd, file_fd) == -1)
		ft_end_program(shell, 1, errno);	
	if (file_fd == 1)
		shell->outfile = fd;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}
