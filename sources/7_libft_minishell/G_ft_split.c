/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   F_ft_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:07:54 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 17:50:45 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char		**ft_spliting(char **tab, t_split sp, t_shell *shell);
static size_t	ft_strclen(const char *s, int c);
static int		ft_nb_lignes(char const *s, char c, int i);

char	**ft_split(char *s, char c, t_shell *shell)
{
	int		i;
	int		lignes;
	char	**tab;
	t_split	sp;

	i = 0;
	sp.c = c;
	sp.s = s;
	if (s == NULL)
		return (NULL);
	lignes = ft_nb_lignes(s, c, i);
	tab = (char **)ft_calloc((lignes + 1) * sizeof(char *), shell);
	if (tab == NULL)
		return (NULL);
	if (lignes == 1)
	{
		tab[0] = s;
		tab[1] = NULL;
		return (tab);
	}
	sp.lignes = lignes;
	ft_spliting(tab, sp, shell);
	tab[lignes] = NULL;
	return (tab);
}

static int	ft_nb_lignes(char const *s, char c, int i)
{
	int	lignes;

	lignes = 0;
	while (s[i] && s[i] == c)
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		while (s[i] && s[i] != c)
			i++;
		lignes++;
		if (!s[i])
			return (lignes);
		while (s[i] && s[i] == c)
			i++;
	}
	return (lignes);
}

static char	**ft_spliting(char **tab, t_split sp, t_shell *shell)
{
	int	i;
	int	j;
	int	len;
	int	w;

	i = 0;
	j = 0;
	while (j < sp.lignes)
	{
		w = -1;
		while (sp.s[i] == sp.c)
			i++;
		len = ft_strclen(&sp.s[i], (int) sp.c);
		tab[j] = (char *)ft_calloc((len + 1) * sizeof(char), shell);
		if (tab[j] == 0)
			return (NULL);
		while (++w < len)
				tab[j][w] = sp.s[i++];
		tab[j][w] = '\0';
		j++;
	}
	return (tab);
}

static size_t	ft_strclen(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}
