/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:41:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/22 16:41:02 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_dollar_sign(t_shell *shell, int i, int y)
{
	char	**tab;
	int		**is_quoted;
	// char	*part1;
	// char	*part2;
	char	*new;

	tab = shell->tab;
	is_quoted = shell->is_quoted;
	if (tab[i][y + 1] && tab[i][y + 1] == '?' && !is_quoted[i][y + 1])
	{
		if (y == 0)
		{
			new = ft_strcat(ft_itoa(exit_true_status, shell), \
				&tab[i][y + 2], shell);
			// printf("cc %s\n", new);
			shell->tab[i] = new;
			// if (i == 0 && !tab[1])
			// 	return (write(2, new, ft_strlen(new)), write(2, "\n", 2), -1);
			// shell->is_quoted[i] = ft_calloc(sizeof(int) * (ft_strlen(new)))
			// int	z;
			// z = -1;
			// while (shell->is_quoted[i][++z] != IS_QUOTED_END)
			// 	printf("%d\n", shell->is_quoted[i][++z]);
			
		}
	}
	return (y);
}

// static int	*new_is_quoted(char *new, int *old_isq, t_shell *shell)
// {
// 	int	*new_iq;
// 	int	i;
	
	
// }
int	ft_find_exit_status(t_shell *shell)
{
	int	i;
	int	y;

	i = -1;
	while (shell->tab[++i])
	{
		y = -1;
		while (shell->tab[i][++y])
		{
			if (shell->tab[i][y] == '$' && !shell->is_quoted[i][y])
				y = ft_dollar_sign(shell, i, y);
			if (y == -1)
				return (1);
			if (!shell->tab[i][y])
				break ;
		}
	}
	return (0);
}