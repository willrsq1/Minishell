/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minishell_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:12:41 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/08 23:37:13 by wruet-su         ###   ########.fr       */
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
		s[i] == ';'))
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
		s[i] != '"' && s[i] != '\'' && s[i] != '|' && s[i] != '&'&& s[i] != ';')
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

int	ft_should_i_break(char *s, int i)
{
	if ((i > 0 && !s[i - 1]) || !s[i] || s[i] == ' ' || s[i - 1] == '<' \
		|| s[i - 1] == ';' || \
		s[i - 1] == '>' || s[i - 1] == '|' || s[i - 1] == '&')
		return (1);
	if ((s[i - 1] == '"' || s[i - 1] == '\'') && (s[i] == '<' || s[i] == '>'))
		return (1);
	if (s[i] == '|' || s[i] == '<' || s[i] == '>' || s[i] == '&' || s[i] == ';')
		return (1);
	return (0);
}
