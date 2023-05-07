/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:51:06 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/01 18:21:53 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_ppx_strncmp_path(char *str, char *checked, int size)
{
	int	i;

	if (!str || !checked)
		return (0);
	i = 0;
	while (str[i] && checked[i] && str[i] == checked[i])
		i++;
	if (i == size)
		return (1);
	return (0);
}

char	*ft_strjoin_ppx(char *s1, char *s2, t_pipex *p)
{
	char	*str;
	int		i;
	int		y;

	i = -1;
	y = -1;
	if (!s1)
		return (s2);
	if (!s2)
		return (NULL);
	if (!s1 && !s2)
		return (NULL);
	str = ft_calloc(sizeof(char) * (ft_strlen(s1) \
		+ ft_strlen(s2) + 1), p->shell);
	while (s1[++i])
		str[i] = s1[i];
	while (s2[++y])
		str[i + y] = s2[y];
	str[i + y] = '\0';
	return (str);
}

void	ft_get_paths_in_struct(t_pipex *p, char **envp)
{
	int		i;
	char	*envp2;

	i = 0;
	while (envp[i] && ft_ppx_strncmp_path(envp[i], "PATH=", 5) == 0)
		i++;
	envp2 = envp[i] + 5;
	p->paths = ft_split(envp2, ':', p->shell);
}
