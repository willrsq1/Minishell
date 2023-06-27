/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 02:30:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/27 00:23:34 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_cd_no_args(t_shell *shell);
static void	ft_cd_too_many_args_error(void);
static void	ft_cd_one_arg(char **tab, t_shell *shell);

int	ft_cd(t_shell *shell, char **tab)
{
	if (!tab[1])
		ft_cd_no_args(shell);
	else if (tab[2])
		ft_cd_too_many_args_error();
	else
		ft_cd_one_arg(tab, shell);
	return (0);
}

static void	ft_cd_no_args(t_shell *shell)
{
	char	old_pwd[FILENAME_MAX];
	char	new_pwd[FILENAME_MAX];

	getcwd(old_pwd, FILENAME_MAX);
	if (chdir(getenv("HOME")) == FAIL)
	{//add error if getenvfail, get our own getenv bc HOME never changes
		g_exit_code = ERROR;
		perror("Minishell: cd: HOME: ");
	}
	getcwd(new_pwd, FILENAME_MAX);
	shell->tab[1] = ft_strcat("OLDPWD=", old_pwd, shell);
	shell->tab[2] = NULL;
	ft_export(shell, shell->tab, shell->envp);
	shell->tab[1] = ft_strcat("PWD=", new_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
}

static void	ft_cd_too_many_args_error(void)
{
	write(2, "Minishell: cd: Too many arguments\n", 35);
	g_exit_code = ERROR;
}

static void	ft_cd_one_arg(char **tab, t_shell *shell)
{
	char	*old_pwd;
	char	*new_pwd;

	g_exit_code = OK;
	old_pwd = ft_calloc(sizeof(char) * FILENAME_MAX, shell);
	new_pwd = ft_calloc(sizeof(char) * FILENAME_MAX, shell);
	if (!getcwd(old_pwd, FILENAME_MAX))
		perror("Minishell: getcwd");
	if (!tab[1][0] || chdir(tab[1]) == FAIL)
	{
		perror(ft_strcat("Minishell: chdir: ", tab[1], shell));
		g_exit_code = ERROR;
		return ;
	}
	if (!getcwd(new_pwd, FILENAME_MAX))
		perror("Minishell: getcwd");
	shell->tab[1] = ft_strcat("OLDPWD=", old_pwd, shell);
	shell->tab[2] = NULL;
	ft_export(shell, shell->tab, shell->envp);
	shell->tab[1] = ft_strcat("PWD=", new_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
}
