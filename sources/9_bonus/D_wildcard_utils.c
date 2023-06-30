/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_wildcard_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 18:11:06 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 13:35:29 by wruet-su         ###   ########.fr       */
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

char	*next_tok(char *arg, t_shell *shell)
{
	int		i;
	char	*new;

	i = 0;
	while (arg[i] && arg[i] != '*')
		i++;
	new = ft_calloc(sizeof(char) * (i + 1), shell);
	i = -1;
	while (arg[++i] && arg[i] != '*')
		new[i] = arg[i];
	return (new);
}

int	ft_check_wildcard_token(char *file, char *new_tok)
{
	int	i;

	i = 0;
	while (new_tok[i])
	{
		if (file[i] != new_tok[i])
			return (1);
		i++;
	}
	return (OK);
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
			return (ERROR);
	}
	return (OK);
}
