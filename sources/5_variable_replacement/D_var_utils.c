/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_var_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 15:27:41 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 11:47:43 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_get_var_name(t_shell *shell, char *arg, int *is_quoted)
{
	char	*var;
	int		i;

	if (!arg[1])
		return (NULL);
	i = 1;
	while (arg[i] && (arg[i] != '$' && is_quoted[i] != 2) && \
		arg[i] != ' ' && arg[i] != '=')
		i++;
	if (i == 1 && arg[1] == ' ')
		return (NULL);
	if (!arg[i])
		var = ft_strdup(&arg[1], shell);
	else
	{
		var = ft_calloc(sizeof(char) * (1 + i), shell);
		while (i-- > 1)
			var[i - 1] = arg[i];
	}
	return (var);
}

int	ft_var_get_envp_lign(char **envp, char *var_name)
{
	int	i;
	int	y;

	i = -1;
	while (envp && envp[++i])
	{
		y = 0;
		while (var_name[y] && envp[i][y] && var_name[y] == envp[i][y])
		{
			y++;
		}
		if (!var_name[y] && envp[i][y] == '=')
			return (i);
	}
	return (FAIL);
}

void	ft_remove_one_token(t_shell *shell, int i, int y)
{
	if (y == -1 && !shell->tab[i][0])
	{
		while (shell->tab[i + 1])
		{
			shell->is_quoted[i] = shell->is_quoted[i + 1];
			shell->tab[i] = ft_strdup(shell->tab[i + 1], shell);
			i++;
		}
		shell->tab[i] = NULL;
		shell->is_quoted[i] = NULL;
	}
}

int	ft_var_not_found(char *arg, int *is_quoted)
{
	int	i;
	int	y;

	i = 1;
	if (arg[1] == '$' && is_quoted[1] != 2)
	{
		printf("PROGRAM ID COULDN'T BE FOUND !\n");
		return (arg[0] = 'I', arg[1] = 'D', 1);
	}
	while (arg[i] && (arg[i] != '$' && is_quoted[i] != 2) && arg[i] != '=')
		i++;
	if (!arg[i])
		return (arg[0] = '\0', is_quoted[0] = IS_QUOTED_END, 2);
	y = -1;
	while (arg[++y + i])
	{
		arg[y] = arg[y + i];
		is_quoted[y] = is_quoted[i + y];
	}
	return (arg[y] = '\0', is_quoted[y] = IS_QUOTED_END, 0);
}

char	**ft_export_sorted_tab(t_shell *shell, char **envp)
{
	char	**tab;
	char	*temp;
	int		i;
	int		y;

	tab = ft_calloc(sizeof(char *) * FILENAME_MAX, shell);
	i = -1;
	while (envp[++i])
		tab[i] = ft_strdup(envp[i], shell);
	while (i-- > 0)
	{
		y = 0;
		while (tab[++y])
		{
			if (tab[y - 1][0] > tab[y][0] || \
				(tab[y - 1][0] == tab[y][0] && tab[y - 1][1] > tab[y][1]))
			{
				temp = tab[y - 1];
				tab[y - 1] = tab[y];
				tab[y] = temp;
			}
		}
	}
	return (tab);
}
