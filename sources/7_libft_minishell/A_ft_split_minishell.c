/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ft_split_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:58:18 by wruet-su          #+#    #+#             */
/*   Updated: 2023/09/07 23:58:19 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_nb_lignes(char *s, char c, int i, t_shell *shell);
static int	nb_lignes_2(int *lignes, int i, char *s, char c);
static char	**ft_spliting(char **tab, t_shell *shell, char c, int lignes);
static int	ft_spliting_2(t_shell *shell, char **tab, t_split split, int i);

char	**ft_split_minishell(char *s, char c, t_shell *shell)
{
	int		i;
	int		lignes;
	char	**tab;
	t_split	split;

	i = 0;
	if (s == NULL)
		return (NULL);
	shell->split = &split;
	lignes = ft_nb_lignes(s, c, i, shell);
	tab = (char **)ft_calloc((lignes * 3 + 1) * sizeof(char *), shell);
	tab[lignes * 2] = NULL;
	shell->buff = s;
	if (ft_spliting(tab, shell, c, lignes * 3) == NULL)
		return (NULL);
	return (tab);
}

static int	ft_nb_lignes(char *s, char c, int i, t_shell *shell)
{
	int		lignes;

	lignes = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		while (s[i] && (s[i] == c || s[i] == '\t'))
			i++;
		if (!s[i])
			return (lignes);
		shell->split->redi_fd = 0;
		shell->split->redi_fd = ft_find_redi_with_fd(s, i);
		if (shell->split->redi_fd != 0 && ++lignes)
			i += shell->split->redi_fd;
		else
			i = nb_lignes_2(&lignes, i, s, c);
	}
	return (lignes);
}

static int	nb_lignes_2(int *lignes, int i, char *s, char c)
{
	int		y;
	char	quote;

	quote = '"';
	y = 0;
	while (s[i] && !((s[i] == ' ' || s[i] == '\t') && y % 2 == 0))
	{
		if (y == 0 && (s[i] == '"' || s[i] == '\''))
			quote = s[i];
		if (s[i] == quote)
			y++;
		if (y % 2 == 0 && ft_is_meta_carac(s[i]) == OK)
		{
			if (s[i + 1] && s[i + 1] != c && ft_is_meta_carac(s[i + 1] == OK))
				*lignes += 1;
			if (i > 0 && !(s[i - 1] == ' ' || ft_is_meta_carac(s[i - 1]) == OK \
				|| s[i - 1] == '\t'))
				*lignes += 1;
			i++;
			break ;
		}
		i++;
	}
	*lignes += 1;
	return (i);
}

static char	**ft_spliting(char **tab, t_shell *s, char c, int lignes)
{
	int		i;
	t_split	split;

	i = 0;
	split.j = -1;
	while (++split.j < lignes)
	{
		if (i > 0 && !s->buff[i - 1])
			break ;
		split.w = -1;
		while (s->buff[i] && (s->buff[i] == c || s->buff[i] == '\t'))
			i++;
		if (!s->buff[i])
			break ;
		split.len = ft_find_redi_with_fd((char *)s->buff, i);
		if (split.len != 0)
		{
			tab[split.j] = (char *)ft_calloc((split.len + 1), s);
			while (++split.w < split.len)
				tab[split.j][split.w] = s->buff[i++];
		}
		else
			i = ft_spliting_2(s, tab, split, i);
	}
	return (tab);
}

static int	ft_spliting_2(t_shell *shell, char **tab, t_split split, int i)
{
	char	*s;
	int		quote;

	s = shell->buff;
	while (1)
	{
		quote = 0;
		if (s[i] == '\'' || s[i] == '"')
		{
			split.len = ft_len_within_quotes(&s[i]);
			quote = 1;
		}
		else if (s[i])
			split.len = ft_len_without_quotes(&s[i], shell);
		tab[split.j] = ft_weird_realloc_thing(tab[split.j], split.len, shell);
		while (split.len >= 0 && split.len--)
		{
			if (s[i] == '(' && quote != 1)
				return (parenthesis(split, shell, tab, i));
			tab[split.j][++split.w] = s[i];
			i++;
		}
		if (ft_break_split_loop(s, i, shell) == OK)
			return (i);
	}
}
