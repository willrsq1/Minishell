/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_var_search.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:41:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 15:36:16 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_var(t_shell *shell, char *arg, int *is_quoted, int y);

int	ft_variables_substitution(t_shell *shell)
{
	int	i;
	int	y;

	i = -1;
	while (shell->tab[++i])
	{
		y = -1;
		while (shell->tab[i][++y])
		{
			if (shell->tab[i][y] == '$' && shell->is_quoted[i][y] != 2)
			{
				if (shell->tab[i][y + 1] == '?')
					ft_dollar_question_mark(shell, i, y, ft_strlen(shell->tab[i]));
				else
				{
					shell->i = i;
					y = ft_var(shell, shell->tab[i], shell->is_quoted[i], y);
				}
			}
		}
		if (y == 0)
			ft_remove_one_token(shell, i--);
	}
	return (0);
}

static int	ft_var(t_shell *shell, char *arg, int *is_quoted, int y)
{
	char	*var;
	int		envp_lign;
	int		i;
	
	var = ft_get_var_name(shell, &arg[y], &is_quoted[y]);
	if (!var)
		return (y);
	envp_lign = ft_var_get_envp_lign(shell->envp, var);
	if (envp_lign == FAIL)
	{
		ft_var_not_found(&arg[y], &is_quoted[y]);
		return (y - 1);
	}
	i = 0;
	while (shell->envp[envp_lign][i] != '=')
		i++;
	var = ft_strdup(&shell->envp[envp_lign][i + 1], shell);
	ft_substitute_var(shell, var, arg, is_quoted);
	return (y);
}
