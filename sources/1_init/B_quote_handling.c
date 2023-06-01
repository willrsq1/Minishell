/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_quote_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 03:44:56 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/01 10:13:17 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	**is_quoted_int_tab_creation(char **tab, t_shell *shell);
static char	**fill_is_quoted(char **tab, int **is_quoted);
static void	fill_is_quoted_loop(char **tab, int **is_quoted, int i, char c);

char	**quotes_management(t_shell *shell, char **tab)
{
	char	**new_tab;

	shell->is_quoted = NULL;
	new_tab = NULL;
	shell->is_quoted = is_quoted_int_tab_creation(tab, shell);
	new_tab = fill_is_quoted(tab, shell->is_quoted);
	return (new_tab);
}

static int	**is_quoted_int_tab_creation(char **tab, t_shell *shell)
{
	int		**is_quoted_tab;
	int		lign;
	int		carac;

	carac = 0;
	lign = 0;
	while (tab[lign])
		lign++;
	is_quoted_tab = ft_calloc(sizeof(int *) * (lign + 1), shell);
	lign = 0;
	while (tab[lign])
	{
		carac = 0;
		while (tab[lign][carac])
			carac++;
		is_quoted_tab[lign] = ft_calloc(sizeof(int) * (carac + 1), shell);
		lign++;
	}
	return (is_quoted_tab);
}

static char	**fill_is_quoted(char **tab, int **is_quoted)
{
	int	i;

	i = -1;
	while (tab[++i])
	{
		if (tab[i][0] != '(')
			fill_is_quoted_loop(tab, is_quoted, i, ft_which_quote(tab[i]));
	}
	return (tab);
}

static void	fill_is_quoted_loop(char **tab, int **is_quoted, int i, char c)
{
	int	quote;
	int	y;

	quote = 0;
	y = -1;
	while (tab[i][++y])
	{
		if (tab[i][y] == c && quote % 2 == 1 && ++quote)
		{
			ft_delete_char(&tab[i][y]);
			c = ft_which_quote(&tab[i][y]);
			if (tab[i][y] == c && quote++ > -1)
				ft_delete_char(&tab[i][y--]);
		}
		else if (tab[i][y] == c && quote % 2 == 0 && ++quote)
			ft_delete_char(&tab[i][y--]);
		else
		{
			is_quoted[i][y] = quote % 2;
			if (is_quoted[i][y])
				is_quoted[i][y] += c % 2;
		}
	}
	is_quoted[i][y] = -42;
}

char	ft_which_quote(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (str[i]);
	}
	return ('"');
}
