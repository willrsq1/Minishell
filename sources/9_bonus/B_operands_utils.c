/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_operands_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:59:40 by wruet-su          #+#    #+#             */
/*   Updated: 2023/09/07 23:59:41 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * FT_IS_IT_OPERAND
 * WILL RETURN A POSITIVE VALUE DESCRIBING THE OPERAND IF THERE IS ONE
 * IS USED TO GET THE OPTIONS FOR EXECUTION WHEN AN OPERAND IS PRESENT
*/

int	ft_is_it_operand(char *arg, int *is_quoted, t_shell *shell)
{
	if (ft_strcmp_unquoted(arg, "||", is_quoted) == OK)
		return (OR_OPERAND);
	if (ft_strcmp_unquoted(arg, "&&", is_quoted) == OK)
		return (AND_OPERAND);
	if (shell->enable_semicolons && \
		ft_strcmp_unquoted(arg, ";", is_quoted) == OK)
		return (SEMICOLON);
	return (OK);
}

/**
 * FT_COUNT_OPERANDS
 * WILL COUNT HOW MANY OPERANDS ARE PRESENT IN THE INPUTS
 * THE RETURN VALUE IS USED TO CREATE A ***CHAR OF ITS SIZE
*/

int	ft_count_operands(t_shell *shell)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (shell->tab[++i])
	{
		if (shell->tab[i][0] == '(' && !shell->is_quoted[i][0])
		{
			shell->is_quoted[i][ft_strlen(shell->tab[i])] = IS_QUOTED_END;
			shell->is_quoted[i] = &shell->is_quoted[i][1];
			shell->tab[i][ft_strlen(shell->tab[i]) - 1] = '\0';
			shell->tab[i] = &shell->tab[i][1];
			count++;
		}
		if (ft_is_it_operand(shell->tab[i], shell->is_quoted[i], shell))
			count++;
	}
	return (count);
}

/**
 * FT_FIND_OP_LENGHT
 * WILL COUNT THE NUMBER OF TOKENS PRESENT BEFORE AN OPERAND
 * THE RETURN VALUE IS USED TO CREATE A **CHAR OF ITS SIZE
*/

int	ft_find_op_lenght(char **tab, t_shell *shell, int i)
{
	int	lenght;

	lenght = 0;
	while (tab[i])
	{
		if (ft_is_it_operand(tab[i], shell->is_quoted[i], shell))
			return (lenght);
		lenght++;
		i++;
	}
	return (lenght);
}
