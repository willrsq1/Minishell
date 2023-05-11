/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 20:56:19 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 13:17:08 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**ft_create_tab(t_shell *shell, char *buff);
static int	ft_count_quotes(char *buff);
static void	count_quotes_perror(int quote, char c);
static char	**fix_tab(t_shell *shell, char **tab);

int	tab_creation(t_shell *shell, char *buff)
{
	char	**tab;

	shell->tab = NULL;
	tab = ft_create_tab(shell, buff);
	if (tab == NULL)
		return (1);
	tab = fix_tab(shell, tab);
	tab = trim_and_is_quoted(shell, tab);
	shell->tab = tab;
	shell->buff = buff;
	if (ft_syntax_error(tab, shell) == 1)
		return (1);
	return (0);
}

static char	**ft_create_tab(t_shell *shell, char *buff)
{
	char	**tab;
	int		nb_db_quotes;

	if (!buff || !buff[0])
		return (NULL);
	nb_db_quotes = ft_count_quotes(buff);
	if (!buff || nb_db_quotes % 2 != 0)
		return (NULL);
	tab = ft_split_minishell(buff, ' ', shell);
	return (tab);
}

static int	ft_count_quotes(char *buff)
{
	int		i;
	int		quote;
	char	c;

	i = 0;
	quote = 0;
	c = ft_which_quote(buff);
	while (buff[i])
	{
		if (buff[i] == c && quote % 2 == 1 && ++quote)
		{
			c = ft_which_quote(&buff[++i]);
			if (buff[i] && buff[i] == c)
				quote++;
		}
		else if (buff[i] == c && quote % 2 == 0)
			quote++;
		if (quote >= INT32_MAX - 3)
			return (write(2, "Too many dbquotes\n", 19), 0);
		if (!buff[i])
			break ;
		i++;
	}
	count_quotes_perror(quote, c);
	return (quote);
}

static void	count_quotes_perror(int quote, char c)
{
	if (quote % 2 != 0)
	{
		write(2, "Minishell: Expected finishing ", 31);
		if (c == '\'')
			write(2, "simple quote.\n", 15);
		if (c == '"')
			write(2, "double quote.\n", 15);
	}
}

static char	**fix_tab(t_shell *shell, char **tab)
{
	int		i;
	char	**fixed;
	int		y;

	i = 0;
	while (tab[i] && tab[i][0])
		i++;
	fixed = ft_calloc(sizeof(char *) * (i + 1), shell);
	y = -1;
	while (++y < i)
		fixed[y] = tab[y];
	fixed[y] = NULL;
	return (fixed);
}
