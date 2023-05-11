/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_functs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:49:53 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 15:36:27 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_tbc_list(int fd, t_shell *shell)
{
	t_to_be_closed	*lst;
	t_to_be_closed	*temp;

	lst = ft_calloc(sizeof(t_to_be_closed), shell);
	lst->next = NULL;
	lst->fd = -1;
	lst->fd = fd;
	if (shell->tbc_list == NULL)
	{
		shell->tbc_list = lst;
		return ;
	}
	temp = shell->tbc_list;
	while (temp && temp->next)
		temp = temp->next;
	temp->next = lst;
}

int	ft_open_rdonly(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Minishell: ", 12);
		perror(file);
		ft_end_program(shell, 0, 1);
		return (fd);
	}
	ft_add_tbc_list(fd, shell);
	return (fd);
}

int	ft_open_append(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 000644);
	if (fd == -1)
	{
		write(2, "Minishell: ", 12);
		perror(file);
		ft_end_program(shell, 0, 1);
		return (fd);
	}
	ft_add_tbc_list(fd, shell);
	return (fd);
}

int	ft_open_trunc(char *file, t_shell *shell)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 000644);
	if (fd == -1)
	{
		write(2, "Minishell: ", 12);
		perror(file);
		ft_end_program(shell, 0, 1);
	}
	ft_add_tbc_list(fd, shell);
	return (fd);
}

void	ft_pipe(int *tab, t_shell *shell)
{
	if (pipe(tab) == -1)
		ft_end_program(shell, 1, errno);
	ft_add_tbc_list(tab[0], shell);
	ft_add_tbc_list(tab[1], shell);
}
