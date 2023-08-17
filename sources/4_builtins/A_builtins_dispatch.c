/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_builtins_dispatch.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:23 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 16:03:08 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_is_builtin_check(char **tab, int i);
static int	ft_is_a_builtin_redirection(t_shell *shell);
static int	ft_builtin_redirection_with_fd(char *arg, int i, t_shell *shell);

/*	Executes builtins outside of child processes when exec_no_pipes.
	Performs the redirections if needed AND if builtins are to be executed.
	If the redirections are bad, everything will be freed.
	The prog won't exit, but we will return OK and end the execution. */

int	ft_builtins(t_shell *shell, char **tab, char **envp)
{
	if (ft_is_a_builtin_redirection(shell) == OK)
	{
		shell->no_exit = 1;
		ft_get_redi(shell);
		if (!shell->to_be_freed_list)
			return (OK);
		shell->no_exit = 0;
		ft_dup2_execution_no_pipes(shell);
	}
	if (ft_strcmp(tab[0], "exit") == OK)
		return (ft_exit(shell, envp));
	g_exit_code = OK;
	if (ft_strcmp(tab[0], "cd") == OK)
		return (ft_cd(shell, tab));
	if (ft_strcmp(tab[0], "export") == OK)
		return (ft_export(shell, tab, envp));
	if (ft_strcmp(tab[0], "unset") == OK)
		return (ft_unset(shell, tab, envp));
	return (1);
}

/*	These builtins are those who can be executed in childs.
	We don't care about redirections because they are already done in child. */

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

/*	This will return ERROR if the input isn't:
	-> One of the four builtins that need to be executed outside of child.
	IF a redirection is found, it's skipped (i = i + 1).
	i += 1 skips it because a redirection is made of two tokens. */

static int	ft_is_a_builtin_redirection(t_shell *shell)
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

/*	This part checks for specific fds redirections, not only STDOUT. */

static int	ft_builtin_redirection_with_fd(char *arg, int i, t_shell *shell)
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

/*	Used in ft_is_a_builtin_redirection. Stops the check, if: 
	A builtin gets found. */

static int	ft_is_builtin_check(char **tab, int i)
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
