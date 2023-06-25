/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_builtins_dispatch.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:23 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/24 11:36:48 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtins_redirections(t_shell *shell, int i, int y, char **tab)
{
	pid_t	pid;

	while (tab[i + y] && \
		ft_is_token_redi(tab[i + y], shell->is_quoted[i + y]) == ERROR)
	{
		if (!tab[i + y])
			return (ERROR);
		if (ft_strcmp(tab[i + y], "exit") == OK || \
			ft_strcmp(tab[i + y], "cd") == OK || \
			ft_strcmp(tab[i + y], "export") == OK)
			break ;
		y += 2;
	}
	if (!tab[i + y])
		return (ERROR);
	pid = fork();
	if (pid == FAIL)
		ft_end_program(shell, ERROR, ERROR);
	if (pid == 0)
	{
		ft_get_redi(shell);
		ft_end_program(shell, OK, OK);
	}
	i = 0;
	waitpid(pid, &i, 0);
	if (WIFEXITED(i))
		exit_true_status = WEXITSTATUS(i);
	return (OK);
}
int	ft_builtins(t_shell *shell, char **tab, char **envp)
{
	int	i;

	i = 0;
	exit_true_status = OK;
	shell->no_exit = 1;
	ft_get_redi(shell);
	if (!shell->to_be_freed_list)
		return (OK);
	shell->no_exit = 0;
	ft_dup2_exec_no_pipes(shell);
	if (ft_strcmp(shell->tab[i], "exit") == OK)
		return (ft_exit(shell, envp));
	if (ft_strcmp(tab[i], "cd") == OK)
		return (ft_cd(shell, tab));
	if (ft_strcmp(tab[i], "export") == OK)
		return (ft_export(shell, tab, envp));
	if (ft_strcmp(tab[i], "unset") == OK)
		return (ft_unset(shell, tab, envp));
	return (1);
}

int	ft_builtins_in_child(t_shell *shell, char **tab, char **envp)
{
	if (ft_strcmp(tab[0], "pwd") == OK)
		return (ft_pwd());
	if (ft_strcmp(tab[0], "env") == OK)
		return (ft_env(envp));
	if (ft_strcmp(tab[0], "echo") == OK)
		return (ft_echo(tab));
	if (shell)
		return (1);
	return (1);
}
