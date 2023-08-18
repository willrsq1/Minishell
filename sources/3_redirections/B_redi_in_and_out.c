/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_redi_in_and_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 21:08:45 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/18 14:20:33 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	OPEN RDONLY, returns the fd to the infile variable, removes the tokens. */

void	ft_infile(t_shell *shell, int i)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open(tab[i + 1], shell, RDONLY);
	if (fd == FAIL)
		return ;
	shell->infile = fd;
	ft_remove_two_tokens(shell, i);
	return ;
}

/*	OPEN APPEND OR TRUNC, returns the fd, removes the tokens of the redi. */

void	ft_outfile(t_shell *shell, int i, int option)
{
	char	**tab;
	int		fd;

	tab = shell->tab;
	fd = ft_open(tab[i + 1], shell, option);
	if (fd == FAIL)
		return ;
	ft_remove_two_tokens(shell, i);
	shell->outfile = fd;
	return ;
}

/*	Open the file specified, dup2 the fd onto it. */

void	ft_infile_with_fd(t_shell *shell, int i, int file_fd)
{
	int		fd;

	if (file_fd == FAIL)
		return ;
	fd = ft_open(shell->tab[i + 1], shell, RDONLY);
	if (fd == FAIL)
		return ;
	ft_remove_two_tokens(shell, i);
	if (dup2(fd, file_fd) == FAIL)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	if (file_fd == 0)
		shell->infile = file_fd;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}

/*	Open the file specified, dup2 the fd onto it. */

void	ft_outfile_with_fd(t_shell *shell, int i, int file_fd, int option)
{
	int	fd;

	if (file_fd == FAIL)
		return ;
	fd = ft_open(shell->tab[i + 1], shell, option);
	if (fd == FAIL)
		return ;
	ft_remove_two_tokens(shell, i);
	if (dup2(fd, file_fd) == FAIL)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	if (file_fd == 1)
		shell->outfile = file_fd;
	ft_add_tbc_list(file_fd, shell);
	ft_add_tbc_list(fd, shell);
	return ;
}
