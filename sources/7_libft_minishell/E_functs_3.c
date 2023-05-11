/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_functs_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:13:33 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/12 01:23:18 by wruet-su         ###   ########.fr       */
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

// char *ft_join_tab(char **tab, t_shell *shell)
// {
// 	char *new;
// 	int i;
// 	i = -1;
// 	int y;
// 	int c;
// 	int w;
// 	new = ft_calloc(10000, shell);
// 	w = -1;
// 	c = 0;
// 	while (tab[++i])
// 	{
// 		y = -1;
// 		while (tab[i][++y])
// 		{
// 			if (c && !shell->is_quoted[i][y])
// 			{
// 				c = 0;
// 				new[++w] = '"';
// 			}
// 			if (shell->is_quoted[i][y] && c == 0 && ++c)
// 				new[++w] = '"';
// 			new[++w] = tab[i][y];
// 		}
// 		if (c && !shell->is_quoted[i][y])
// 		{
// 			c = 0;
// 			new[++w] = '"';
// 		}
// 		new[++w] = ' ';
// 	}
// 	return (new);
// }
static int	ft_lenght_join_tab(char **tab, t_shell *shell, int i, int y)
{
	int	lenght;
	int	quoted;

	lenght = 0;
	while (tab[++i])
	{
		y = -1;
		quoted = 0;
		while (tab[++y])
		{
			if (shell->is_quoted[i][y] != quoted && ++lenght)
				quoted = shell->is_quoted[i][y];
			lenght++;
		}
		if (shell->is_quoted[i][y] != quoted && ++lenght)
			quoted = shell->is_quoted[i][y];
	}
	return (lenght);
}

char *ft_join_tab(char **tab, t_shell *shell)
{
	char *new;
	int i;
	i = -1;
	int y;
	int quoted;
	int	c;
	int w;
	new = ft_calloc(ft_lenght_join_tab(tab, shell, -1, -1) + 1, shell);
	w = -1;
	c = 0;
	while (tab[++i])
	{
		y = -1;
		quoted = 0;
		while (tab[i][++y])
		{
			if (c && !shell->is_quoted[i][y])
			{
				c = 0;
				new[++w] = '"';
			}
			if (shell->is_quoted[i][y] && c == 0 && ++c)
				new[++w] = '"';
			new[++w] = tab[i][y];
		}
		if (c && !shell->is_quoted[i][y])
		{
			c = 0;
			new[++w] = '"';
		}
		new[++w] = ' ';
	}
	if (quoted)
		return (new);
	return (new);
}
