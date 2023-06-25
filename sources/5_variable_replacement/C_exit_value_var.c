/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_exit_value_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 15:16:29 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 15:16:39 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_dollar_question_mark(t_shell *shell, int z, int y, int len)
{
	char *arg;
	int *is_quoted;
	char	*nb;
	char	*new;
	int		*new_is_quoted;
	int		i;
	int		nb_len;

	arg = shell->tab[z];
	is_quoted = shell->is_quoted[z];
	new = ft_calloc(sizeof(char) * (len + 5), shell);
	new_is_quoted = ft_calloc(sizeof(int) * (len + 5), shell);
	i = -1;
	while (++i < y && y > 0)
	{
		new[i] = arg[i];
		new_is_quoted[i] = is_quoted[i];
	}
	y += 1;
	nb = ft_itoa(exit_true_status, shell);
	nb_len = -1;
	while (nb[++nb_len])
	{
		new[i] = nb[nb_len];
		new_is_quoted[i++] = 0;
	}
	while (arg[++y])
	{
		new[i] = arg[y];
		new_is_quoted[i] = is_quoted[y];
		i++;
	}
	new_is_quoted[i] = IS_QUOTED_END;
	shell->tab[z] = new;
	shell->is_quoted[z] = new_is_quoted;
}
