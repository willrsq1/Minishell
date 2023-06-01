/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_functs_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:13:33 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/01 18:24:18 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_join_tab_quoting(int *y_add, int i, t_shell *shell, int w);
static int	ft_lenght_join_tab(char **tab, t_shell *shell, int i, int y);

int	ft_is_meta_carac(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '&' || c == '<' || c == '>')
		return (ERROR);
	return (OK);
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
	return (OK);
}

char *ft_join_tab(char **tab, t_shell *shell)
{
	char *new;
	int i;
	i = -1;
	int y;
	int w;
	shell->buff = ft_calloc(ft_lenght_join_tab(tab, shell, -1, -1) + 1, shell);
	new = shell->buff;
	w = -1;
	while (tab[++i])
	{
		y = -1;
		while (tab[i][++y])
		{
			if (shell->is_quoted[i][y])
				w = ft_join_tab_quoting(&y, i, shell, w);
			else
				new[++w] = tab[i][y];
		}
		new[++w] = ' ';
	}
	return (new);
}

static int	ft_join_tab_quoting(int *y_add, int i, t_shell *shell, int w)
{
	int	y;
	int	quote;
	char *new;
	char **tab;

	tab = shell->tab;
	new = shell->buff;
	y = *y_add;
	if (shell->is_quoted[i][y] == 1)
		new[++w] = '"';
	if (shell->is_quoted[i][y] == 2)
		new[++w] = '\'';
	quote = shell->is_quoted[i][y];
	while (shell->is_quoted[i][y] == quote)
		new[++w] = tab[i][y++];
	y--;
	if (quote == 1)
		new[++w] = '"';
	if (quote == 2)
		new[++w] = '\'';
	*y_add = y;
	return (w);
}

static int	ft_lenght_join_tab(char **tab, t_shell *shell, int i, int y)
{
	int	lenght;
	int	quoted;

	lenght = 0;
	while (tab[++i])
	{
		y = -1;
		quoted = 0;
		while (tab[i][++y])
		{
			if (shell->is_quoted[i][y] != quoted && ++lenght)
			{
				if (quoted && shell->is_quoted[i][y])
					lenght++;
				quoted = shell->is_quoted[i][y];
			}
			lenght++;
		}
		if (shell->is_quoted[i][y] != quoted && ++lenght)
			quoted = shell->is_quoted[i][y];
		lenght++;
	}
	return (lenght);
}
