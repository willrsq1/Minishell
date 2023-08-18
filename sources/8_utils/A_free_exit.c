/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_free_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:29:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/19 00:17:52 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_close_perror(t_shell *shell, char *description, int mode)
{
	if (mode == ERROR)
	{
		description = ft_strcat("Minishell", description, shell);
		perror(description);
	}
	ft_close_all_fds(shell);
	ft_clear_memory(shell);
}

void	ft_end_program(t_shell *shell, int mode, int err)
{
	int		i;
	char	**envp;

	if (err == FAIL)
		err = EXIT_FAILURE;
	if (mode == ERROR)
		perror("Minishell: error: fatal");
	g_exit_code = err;
	ft_close_all_fds(shell);
	ft_clear_memory(shell);
	if (shell->no_exit != OK)
		return ;
	rl_clear_history();
	envp = shell->envp;
	i = -1;
	while (envp && envp[++i])
		free(envp[i]);
	if (envp)
		free(envp);
	exit(err);
}

void	ft_clear_memory(t_shell *shell)
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

void	ft_close_all_fds(t_shell *shell)
{
	t_to_be_closed	*lst;

	if (!shell)
		return ;
	dup2(STDOUT_FILENO, STDIN_FILENO);
	dup2(STDIN_FILENO, STDOUT_FILENO);
	lst = shell->tbc_list;
	if (!lst)
		return ;
	while (lst)
	{
		if (lst->fd > 2 && lst->fd < FILENAME_MAX)
			close(lst->fd);
		lst = lst->next;
	}
	shell->tbc_list = NULL;
}
