/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_builtins_dispatch.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:23 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 12:15:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_builtin_check(char **tab, int i)
{
	
	if (ft_strcmp(tab[i], "exit") == OK)
		return (OK);
	if (ft_strcmp(tab[i], "cd") == OK)
		return (OK);
	if (ft_strcmp(tab[i], "export") == OK)
		return (OK);
	if (ft_strcmp(tab[i], "unset") == OK)
		return (OK);
	return (ERROR);
}

int	ft_builtin_redirection_with_fd(char *arg, int i, t_shell *shell)
{
	int		y;

	if (arg_is_unquoted(arg, shell->is_quoted[i]) == ERROR || \
		ft_find_redi_with_fd(arg, 0) == OK)
		return (FAIL);
	y = 0;
	while (arg[y] && (arg[y] >= '0' && arg[y] <= '9'))
		y++;
	if (ft_strcmp(&arg[y], "<<") == OK)
		return (OK);
	else if (ft_strcmp(&arg[y], "<") == OK)
		return (OK);
	else if (ft_strcmp(&arg[y], ">>") == OK)
		return (OK);
	else if (ft_strcmp(&arg[y], ">") == OK)
		return (OK);
	return (ERROR);
}

int	ft_is_a_builtin_redirection(t_shell *shell)
{
	int		i;
	char	**tab;
	int		**is_quoted;

	tab = shell->tab;
	is_quoted = shell->is_quoted;
	i = -1;
	while (tab[++i])
	{
		if (ft_builtin_redirection_with_fd(tab[i], i, shell) == OK)
			i += 1;
		else if (ft_strcmp_unquoted(tab[i], "<<", is_quoted[i]) == OK)
			i += 1;
		else if (ft_strcmp_unquoted(tab[i], "<", is_quoted[i]) == OK)
			i += 1;
		else if (ft_strcmp_unquoted(tab[i], ">>", is_quoted[i]) == OK)
			i += 1;
		else if (ft_strcmp_unquoted(tab[i], ">", is_quoted[i]) == OK)
			i += 1;
		else if (ft_is_builtin_check(tab, i) == OK)
			return (OK);
		else
			return (ERROR);
	}
	return (ERROR);
}

int	ft_builtins(t_shell *shell, char **tab, char **envp)
{
	int	i;

	i = 0;
	
	exit_true_status = OK;
	if (ft_is_a_builtin_redirection(shell) == OK)
	{
		shell->no_exit = 1;
		ft_get_redi(shell);
		if (!shell->to_be_freed_list)
			return (OK);
		shell->no_exit = 0;
		ft_dup2_exec_no_pipes(shell);
	}
	if (ft_strcmp(tab[i], "exit") == OK)
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
