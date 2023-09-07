/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_exit_value_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:57:43 by wruet-su          #+#    #+#             */
/*   Updated: 2023/09/07 23:57:45 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_insert_exit_status(int i, char *new, int *new_is_quoted, \
	t_shell *shell);

void	ft_exit_value_sub(t_shell *shell, int z, int y, int len)
{
	char	*new;
	int		*new_is_quoted;
	int		i;

	new = ft_calloc(sizeof(char) * (len + 5), shell);
	new_is_quoted = ft_calloc(sizeof(int) * (len + 5), shell);
	i = -1;
	while (++i < y && y > 0)
	{
		new[i] = shell->tab[z][i];
		new_is_quoted[i] = shell->is_quoted[z][i];
	}
	y += 1;
	i = ft_insert_exit_status(i, new, new_is_quoted, shell);
	while (shell->tab[z][++y])
	{
		new[i] = shell->tab[z][y];
		new_is_quoted[i] = shell->is_quoted[z][y];
		i++;
	}
	new_is_quoted[i] = IS_QUOTED_END;
	shell->tab[z] = new;
	shell->is_quoted[z] = new_is_quoted;
}

static int	ft_insert_exit_status(int i, char *new, int *new_is_quoted, \
	t_shell *shell)
{
	char	*nb;
	int		nb_len;

	nb = ft_itoa(g_exit_code, shell);
	nb_len = -1;
	while (nb[++nb_len])
	{
		new[i] = nb[nb_len];
		new_is_quoted[i++] = 0;
	}
	return (i);
}
