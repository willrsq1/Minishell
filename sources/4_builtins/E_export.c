/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/24 01:30:45 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_export_no_args(char **envp);
static int	ft_export_check_arg(char *arg);
static int	ft_export_var_location(char **envp, t_shell *shell, char *arg);

int	ft_export(t_shell *shell, char **tab, char **envp)
{
	int		i;
	int		y;
	int		z;

	z = 0;
	exit_true_status = OK;
	if (!envp)
		return (OK);
	if (!tab[1])
		return (ft_export_no_args(envp), OK);
	while (tab[++z])
	{
		i = -1;
		y = -1;
		if (ft_export_check_arg(tab[z]) == ERROR)
			return (OK);
		i = ft_export_var_location(envp, shell, tab[z]);
		if (i == FAIL)
			return (0);
		while (tab[z][++y] && y < 1024)
			envp[i][y] = tab[z][y];
		envp[i][y] = '\0';
	}
	return (OK);
}

static void	ft_export_no_args(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		printf("declare -x %s\n", envp[i]);
	exit_true_status = OK;
}

static int	ft_export_check_arg(char *arg)
{
	int	y;
	
	y = -1;
	if (arg[0] == '=' || arg[0] == 0 || arg[0] == '$')
	{
		write(2, "Minishell: export: `", 21);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 27);
		exit_true_status = ERROR;
		return (ERROR);
	}
	while (arg[++y])
	{
		if ( arg[y] == ' ')
		{
			write(2, "Minishell: export: `", 21);
			write(2, arg, ft_strlen(arg));
			write(2, "': not a valid identifier\n", 27);
			exit_true_status = ERROR;
			return (ERROR);
		}
	}
	return (OK);
}

static int	ft_export_get_envp_lign(char **envp, char *var_name)
{
	int	i;
	int	y;

	i = -1;
	while (envp[++i])
	{
		y = 0;
		while (var_name[y] && envp[i][y] && var_name[y] == envp[i][y])
		{
			if (var_name[y] == '=' || var_name[y + 1] == '=')
				return (i);
			y++;
		}
		if (!var_name[y] && envp[i][y])
			return (FAIL);
		if (!var_name[y] && !envp[i][y])
			return (i);
	}
	return (i);
}

static int	ft_export_var_location(char **envp, t_shell *shell, char *arg)
{
	int		i;
	char	*var_name;

	var_name = ft_calloc(ft_strlen(arg) + 2, shell);
	i = -1;
	while (arg[++i] && arg[i] != '=')
		var_name[i] = arg[i];
	if (arg[i])
		var_name[i] = '=';
	i = ft_export_get_envp_lign(envp, var_name);
	if (i == 936)
		return (write(2, "Max exports allowed reached\n", 29), FAIL);
	if (i != FAIL && envp[i] == NULL)
	{
		envp[i + 1] = NULL;
		if (!shell->pipex)
			envp[i] = malloc(sizeof(char) * 1024);
		else
			envp[i] = ft_calloc(sizeof(char) * 1024, shell);
		if (!envp[i])
			ft_end_program(shell, ERROR, ERROR);
	}
	return (i);
}