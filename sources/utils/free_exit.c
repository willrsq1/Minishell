/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:29:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 13:28:55 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_close_perror(t_shell *shell, char *description, int mode)
{
	if (mode == 1)
	{
		description = ft_strcat("Minishell: ", description, shell);
		perror(description);
	}
	ft_close_everything_lol(shell);
	ft_free_everything_lol(shell);
}

void	ft_end_program(t_shell *shell, int mode, int err)
{
	if (err == -1)
		err = errno;
	if (mode == 1)
		perror("Minishell: ");
	ft_close_everything_lol(shell);
	ft_free_everything_lol(shell);
	exit(err);
}

void	ft_free_everything_lol(t_shell *shell)
{
	t_to_be_freed	*lst;
	t_to_be_freed	*temp;

	if (!shell)
		return ;
	lst = shell->to_be_freed_list;
	if (!lst)
		return ;
	while (lst)
	{
		temp = lst->next;
		if (lst->to_be_freed)
			free (lst->to_be_freed);
		free (lst);
		lst = temp;
	}
	shell->to_be_freed_list = NULL;
}

void	ft_close_everything_lol(t_shell *shell)
{
	t_to_be_closed	*lst;

	if (!shell)
		return ;
	lst = shell->tbc_list;
	if (!lst)
		return ;
	while (lst)
	{
		if (lst->fd && lst->fd > 2 && lst->fd < 1024)
			close(lst->fd);
		lst = lst->next;
	}
	shell->tbc_list = NULL;
}
