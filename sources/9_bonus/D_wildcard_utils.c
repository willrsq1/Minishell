/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:11:06 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 19:10:05 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_wildcnew_wildcard(void *file, t_wildc *first, t_shell *shell)
{
	t_wildc	*lst;

	lst = ft_calloc(sizeof(t_wildc), shell);
	lst->file = file;
	lst->next = NULL;
	if (first->next == NULL)
	{
		first->next = lst;
		return ;
	}
	while (first->next)
		first = first->next;
	first->next = lst;
}

int	ft_is_invalid_arg_wildcard(char *arg, char *file)
{
	int		i;
	int		y;
	char	next_char;

	i = -1;
	while (arg[++i] != '*')
	{
		if (file[i] != arg[i])
			return (-1);
	}
	y = i + 1;
	next_char = arg[y];
	while (file[i] && file[i] != next_char)
		i++;
	if (next_char == '\0' && file[i] == next_char)
		return (0);
	while (arg[y] && arg[y] != '*' && file[i] && arg[y] == file[i++])
		y++;
	// if (arg[y] == '*')
	// 	printf("THIS %s\n", &arg[y]);
	return (arg[y] - file[i]);
}

char	*find_wildcard_directory(t_shell *shell, char *arg, char **arg_add)
{
	int		i;
	char	*path;

	if (arg[0] == '/' || \
		(arg[0] == '.' && arg[1] == '.' && arg[2] == '/' ))
	{
		i = 0;
		while (arg[i] && (arg[i] == '.' || arg[i] == '/'))
			i++;
		path = ft_calloc(i + 1, shell);
		i = -1;
		while (arg[++i] && (arg[i] == '.' || arg[i] == '/'))
			path[i] = arg[i];
		*arg_add = ft_strdup(&arg[i], shell);
		return (path);
	}
	return (ft_strdup(".", shell));
}

int	find_wildcard(char *arg, t_shell *shell, int i)
{
	int	y;

	y = -1;
	while (arg[++y])
	{
		if (arg[y] == '*' && shell->is_quoted[i][y] == 0)
			return (1);
	}
	return (0);
}
