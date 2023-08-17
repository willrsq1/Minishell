/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   F_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:09 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 19:43:42 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_remone_one_token_envp(char **envp, int i)
{
	while (envp[i + 1])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	envp[i] = NULL;
	envp[i + 1] = NULL;
}

int	ft_unset_get_envp_lign(char **envp, char *var_name)
{
	int	i;
	int	y;

	i = -1;
	while (envp[++i])
	{
		y = 0;
		while (var_name[y] && envp[i][y] && var_name[y] == envp[i][y])
		{
			y++;
		}
		if (!var_name[y] && envp[i][y] == '=')
			return (i);
		if (!var_name[y] && !envp[i][y])
			return (i);
	}
	return (FAIL);
}

int	ft_unset(t_shell *shell, char **tab, char **envp)
{
	int	i;
	int	y;

	y = -1;
	g_exit_code = OK;
	while (tab[++y])
	{
		i = ft_unset_get_envp_lign(envp, tab[y]);
		if (i != FAIL)
		{
			free(envp[i]);
			ft_remone_one_token_envp(envp, i);
		}
	}
	if (shell)
		return (OK);
	return (OK);
}
