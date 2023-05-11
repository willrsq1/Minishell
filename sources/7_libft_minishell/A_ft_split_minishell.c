/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_ft_split_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 12:32:43 by marvin            #+#    #+#             */
/*   Updated: 2023/05/11 17:49:35 by wruet-su         ###   ########.fr       */
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
	tab = (char **)ft_calloc((lignes * 2 + 1) * sizeof(char *), shell);
	tab[lignes * 2] = NULL;
	shell->buff = s;
	if (ft_spliting(tab, shell, c, lignes) == NULL)
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
		while (s[i] && s[i] == c)
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
	while (s[i] && !(s[i] == ' ' && y % 2 == 0))
	{
		if (y == 0 && (s[i] == '"' || s[i] == '\''))
			quote = s[i];
		if (s[i] == quote)
			y++;
		if (y % 2 == 0 && ft_is_meta_carac(s[i]) == 1)
		{
			if (s[i + 1] && s[i + 1] != c && ft_is_meta_carac(s[i + 1] == 1))
				*lignes += 1;
			if (i > 0 && !(s[i - 1] == ' ' || ft_is_meta_carac(s[i - 1]) == 1))
				*lignes += 1;
			i++;
			break ;
		}
		i++;
	}
	*lignes += 1;
	return (i);
}

static char	**ft_spliting(char **tab, t_shell *shell, char c, int lignes)
{
	int		i;
	t_split	split;

	i = 0;
	split.j = -1;
	while (++split.j < lignes)
	{
		if (i > 0 && !shell->buff[i - 1])
			break ;
		split.w = -1;
		while (shell->buff[i] && shell->buff[i] == c)
			i++;
		if (!shell->buff[i])
			break ;
		split.len = ft_find_redi_with_fd((char *)shell->buff, i);
		if (split.len != 0)
		{
			tab[split.j] = (char *)ft_calloc((split.len + 1), shell);
			while (++split.w < split.len)
				tab[split.j][split.w] = shell->buff[i++];
		}
		else
			i = ft_spliting_2(shell, tab, split, i);
	}
	return (tab);
}

static int	ft_spliting_2(t_shell *shell, char **tab, t_split split, int i)
{
	char	*s;

	s = shell->buff;
	while (1)
	{
		if (s[i] == '\'' || s[i] == '"')
			split.len = ft_len_within_quotes(&s[i]);
		else if (s[i])
			split.len = ft_len_without_quotes(&s[i]);
		tab[split.j] = ft_weird_realloc_thing(tab[split.j], split.len, shell);
		while (split.len >= 0 && split.len--)
		{
			if (s[i] == '(')
				return (parenthesis(split, shell, tab, i));
			tab[split.j][++split.w] = s[i];
			i++;
		}
		if (ft_should_i_break(s, i) == 1)
			return (i);
	}
}
