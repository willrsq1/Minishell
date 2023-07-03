/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_open_functs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 16:49:53 by wruet-su          #+#    #+#             */
/*   Updated: 2023/07/03 01:43:18 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_add_tbc_list(int fd, t_shell *shell)
{
	t_to_be_closed	*lst;
	static t_to_be_closed	*temp;

	lst = ft_calloc(sizeof(t_to_be_closed), shell);
	lst->next = NULL;
	lst->fd = fd;
	if (shell->tbc_list == NULL)
	{
		temp = lst;
		shell->tbc_list = lst;
		return ;
	}
	// temp = shell->tbc_list;
	// while (temp && temp->next)
	// 	temp = temp->next;
	temp->next = lst;
	temp = temp->next;
}

int	ft_open(char *file, t_shell *shell, int option)
{
	int	fd;

	if (!file && open(".....", O_RDONLY))
		option = FAIL;
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
		file = ft_strcat(file, "\x1b[0m\U0001F605", shell);
		perror(ft_strcat("Minishell: \033[0;31m", file, shell));
		ft_end_program(shell, OK, EXIT_FAILURE);
		return (FAIL);
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
