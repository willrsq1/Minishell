/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 02:30:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/29 00:59:44 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_cd_special_args(t_shell *shell, char *var);
static void	ft_cd_one_arg(char **tab, t_shell *shell);

int	ft_cd(t_shell *shell, char **tab)
{
	if (!tab[1])
		ft_cd_special_args(shell, "HOME");
	else if (tab[2])
	{
		write(2, "Minishell: cd: Too many arguments\n", 35);
		g_exit_code = ERROR;
	}
	else if (ft_strcmp(tab[1], "-") == OK)
		ft_cd_special_args(shell, "OLDPWD");
	else
		ft_cd_one_arg(tab, shell);
	return (0);
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
	if (chdir(tab[1]) == FAIL && tab[1][0])
	{
		perror(ft_strcat("Minishell: cd: ", tab[1], shell));
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

//HOME=
//cd : se passe rien
/*
	HOME
	cd: HOME not set
	pas de HOME
	cd: HOME not set
	cd -
	cd ""
	cd " "
	*/

char	*ft_getenv(char *var, t_shell *shell)
{
	int		lign;
	int		i;

	lign = ft_var_get_envp_lign(shell->envp, var);
	if (lign == FAIL || shell->envp == NULL)
		return (NULL);
	i = 0;
	while (shell->envp[lign][i] && shell->envp[lign][i] != '=')
		i++;
	return (ft_strdup(&shell->envp[lign][i + 1], shell));
}

static void	ft_cd_special_args(t_shell *shell, char *var)
{
	char	old_pwd[FILENAME_MAX];
	char	new_pwd[FILENAME_MAX];
	char 	*buf;

	getcwd(old_pwd, FILENAME_MAX);
	buf = ft_getenv(var, shell);
	if (!buf)
	{
		write(2, "Minishell: cd: ", 16);
		write(2, var, ft_strlen(var));
		write(2, " not set\n", 10);
		g_exit_code = ERROR;
		return ;
	}
	if (ft_strcmp(var, "OLDPWD") == OK)
		printf("%s\n", buf);
	if (chdir(buf) == FAIL && buf[0])
	{
		perror(ft_strcat("Minishell: cd: ", buf, shell));
		g_exit_code = ERROR;
		return ;
	}
	getcwd(new_pwd, FILENAME_MAX);
	shell->tab = ft_split("cd OLDPWD", ' ', shell);
	shell->tab[1] = ft_strcat("OLDPWD=", old_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
	shell->tab[1] = ft_strcat("PWD=", new_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
}
