/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_ft_split_minishell_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:12:41 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/23 17:13:38 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_weird_realloc_thing(char *initial, int added_len, t_shell *shell)
{
	char	*new;
	int		len;

	if (!initial)
	{
		new = ft_calloc(sizeof(char) * (added_len + 1), shell);
		return (new);
	}
	else
	{
		len = 0;
		while (initial[len])
			len++;
		new = ft_calloc(sizeof(char) * (len + added_len + 1), shell);
		len = -1;
		while (initial[++len])
			new[len] = initial[len];
		return (new);
	}
}

int	ft_len_without_quotes(char *s)
{
	int	i;

	i = 0;
	if (s[i] && (s[i] == '<' || s[i] == '>' || s[i] == '|' || s[i] == '&' || \
		s[i] == ';' || s[i] == '$'))
	{
		if (s[i + 1] && s[i] == '<' && s[i + 1] == '<')
			return (2);
		if (s[i + 1] && s[i] == '>' && s[i + 1] == '>')
			return (2);
		if (s[i] && s[i] == '|' && s[i + 1] && s[i + 1] == '|')
			return (2);
		if (s[i] && s[i] == '&' && s[i + 1] && s[i + 1] == '&')
			return (2);
		return (1);
	}
	while (s[i] && s[i] != ' ' && s[i] != '<' && s[i] != '>' && \
		s[i] != '"' && s[i] != '\'' && s[i] != '|' && \
		s[i] != '&' && s[i] != ';' && s[i] != '$')
		i++;
	return (i);
}

int	ft_len_within_quotes(char *s)
{
	char	quote;
	int		i;

	i = 0;
	quote = s[i];
	i = 1;
	while (s[i] && s[i] != quote)
		i++;
	return (i + 1);
}

int	ft_break_split_loop(char *s, int i)
{
	if ((i > 0 && !s[i - 1]) || !s[i] || s[i] == ' ' || s[i - 1] == '<' \
		|| s[i - 1] == ';' || \
		s[i - 1] == '>' || s[i - 1] == '|' || s[i - 1] == '&')
		return (OK);
	if ((s[i - 1] == '"' || s[i - 1] == '\'') && (s[i] == '<' || \
		s[i] == '>'))
		return (OK);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '&' \
		|| s[i] == ';' || (s[i] == '$' && s[i - 1] != '\''))
		return (OK);
	return (ERROR);
}

int	parenthesis(t_split split, t_shell *shell, char **tab, int i)
{
	char	*s;
	int		count;

	count = 0;
	s = shell->buff;
	while (s[i])
	{
		tab[split.j] = ft_weird_realloc_thing(tab[split.j], 1, shell);
		tab[split.j][++split.w] = s[i];
		if (s[i] == ')')
			count--;
		if (s[i] == '(')
			count++;
		i++;
		if (count == 0)
			break ;
	}
	return (i);
}
