/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_var_search.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:47:15 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 13:36:22 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_var(t_shell *shell, char *arg, int *is_quoted, int y);
static int	ft_check_for_ambiguous_redirect(t_shell *shell, int i);

int	ft_variables_substitution(t_shell *shell)
{
	int	i;
	int	y;

	i = -1;
	while (shell->tab[++i])
	{
		y = -1;
		while (shell->tab[i] && shell->tab[i][0] && shell->tab[i][++y])
		{
			if (y == 0 && ft_check_for_ambiguous_redirect(shell, i))
				return (ERROR);
			if (shell->tab[i][y] == '$' && shell->is_quoted[i][y] != 2)
			{
				if (shell->tab[i][y + 1] == '?' && !shell->is_quoted[i][y + 1])
					ft_exit_value_sub(shell, i, y, ft_strlen(shell->tab[i]));
				else
				{
					shell->i = i;
					y = ft_var(shell, shell->tab[i], shell->is_quoted[i], y);
					ft_remove_one_token(shell, i, y);
				}
			}
		}
	}
	return (OK);
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

static int	ft_check_for_ambiguous_redirect(t_shell *shell, int i)
{
	int	y;
	int	**iq;

	iq = shell->is_quoted;
	y = 0;
	if (i == 0)
		return (OK);
	if (!ft_is_token_redi_not_heredoc(shell->tab[i - 1], iq[i - 1]))
		return (OK);
	if (shell->tab[i][y + 1] == '?' && shell->tab[i][y] == '$')
		return (OK);
	while (shell->tab[i][y] && \
		shell->tab[i][y] == '*' && iq[i][y] == OK)
		y++;
	if (shell->tab[i][y] && !(shell->tab[i][0] == '$' && iq[i][0] == OK))
		return (OK);
	write(2, "Minishell: \033[0;31m", 19);
	write(2, shell->tab[i], ft_strlen(shell->tab[i]));
	write(2, "\x1b[0m: ambiguous redirect \U0001F621\n", 31);
	if (shell->pipex)
		ft_end_program(shell, OK, ERROR);
	g_exit_code = ERROR;
	return (ERROR);
}
