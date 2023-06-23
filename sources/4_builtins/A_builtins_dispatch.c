/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_builtins_dispatch.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:23 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/23 02:26:09 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtins(t_shell *shell, char **tab, char **envp)
{
	if (ft_strcmp(shell->tab[0], "exit") == OK)
		return (ft_exit(shell, envp));
	if (ft_strcmp(tab[0], "cd") == OK)
		return (ft_cd(shell, tab));
	if (ft_strcmp(tab[0], "export") == OK)
		return (ft_export(shell, tab, envp));
	if (envp)
		return (1);
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
