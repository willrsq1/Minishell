/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_functs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 03:34:10 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 00:54:38 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(char *arg, char *checked)
{
	int	i;

	if (!arg && !checked)
		return (OK);
	if (!arg || !checked)
		return (ERROR);
	i = 0;
	while (arg[i] && checked[i] && arg[i] == checked[i])
	{
		if (i == INT32_MAX)
			return (write(2, "Arg is too long\n", 17), ERROR);
		i++;
	}
	return (arg[i] - checked[i]);
}

int	ft_strncmp(char *arg, char *checked, int n)
{
	int	i;

	if (!arg && !checked)
		return (OK);
	if (!arg || !checked)
		return (ERROR);
	i = 0;
	while (arg[i] && checked[i] && arg[i] == checked[i] && i < n - 1)
	{
		if (i == INT32_MAX)
			return (write(2, "Arg is too long\n", 17), ERROR);
		i++;
	}
	return (n - 1 - i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i])
		i++;
		return (i);
	}
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

char	*ft_strcat(char *s1, char *s2, t_shell *shell)
{
	char	*str;
	int		len1;
	int		len2;
	int		i;
	int		y;

	if (s2 == NULL)
		return (ft_strdup(s1, shell));
	if (s1 == NULL)
		return (ft_strdup(s2, shell));
	len1 = ft_strlen((char *) s1);
	len2 = ft_strlen((char *) s2);
	str = ft_calloc(len1 + len2 + 1, shell);
	i = -1;
	while (s1 && s1[++i])
		str[i] = s1[i];
	y = -1;
	while (s2 && s2[++y])
		str[i + y] = s2[y];
	str[i + y] = '\0';
	return (str);
}

int	arg_is_unquoted(char *arg, int *is_quoted)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (is_quoted[i])
			return (ERROR);
		i++;
	}
	return (OK);
}
