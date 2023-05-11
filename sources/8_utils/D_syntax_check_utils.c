/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_syntax_check_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 09:29:07 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 20:47:31 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_token_meta(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">") || \
			!ft_strcmp(arg, "|"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>") || \
			!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (1);
	}
	return (0);
}

int	ft_is_token_redi(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>"))
			return (1);
	}
	return (0);
}

int	ft_is_token_operand(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "|") || !ft_strcmp(arg, "&") || !ft_strcmp(arg, ";"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (1);
	}
	return (0);
}
