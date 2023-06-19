/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_open_functs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:49:53 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/19 02:57:40 by wruet-su         ###   ########.fr       */
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

int	ft_open(char *file, t_shell *shell, int option)
{
	int	fd;

	if (option == RDONLY)
		fd = open(file, O_RDONLY);
	else if (option == TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 000644);
	else if (option == APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 000644);
	else
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	if (fd == FAIL)
	{
		// file = ft_strcat(file, "\033[0m", shell);
		perror(ft_strcat("Minishell: ", file, shell));
		ft_end_program(shell, OK, EXIT_FAILURE);
	}
	ft_add_tbc_list(fd, shell);
	return (fd);
}

void	ft_pipe(int *tab, t_shell *shell)
{
	if (pipe(tab) == FAIL)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	ft_add_tbc_list(tab[0], shell);
	ft_add_tbc_list(tab[1], shell);
}
