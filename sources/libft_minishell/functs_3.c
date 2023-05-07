/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functs_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:13:33 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/26 19:02:04 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_is_meta_carac(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '&' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	ft_find_redi_with_fd(char *s, int i)
{
	int						start;
	unsigned long long		atoi;
	int						count;

	count = 0;
	start = i;
	atoi = 0;
	while (s[i] && s[i] >= '0' && s[i] <= '9' && count < 13)
	{
		atoi = atoi * 10 + (s[i] - 48);
		i++;
		count++;
	}
	if (s[i] && (s[i] == '<' || s[i] == '>') && i > start && atoi <= INT32_MAX)
	{
		if (s[i] == '<' && s[i + 1] && s[i + 1] == '<')
			return (i - start + 2);
		if (s[i] == '>' && s[i + 1] && s[i + 1] == '>')
			return (i - start + 2);
		return (i - start + 1);
	}
	return (0);
}
