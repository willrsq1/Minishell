/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 02:17:31 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 10:06:21 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_wildcard(t_shell *shell, char *arg, t_lst *first);

char	**ft_wildcard_loop(t_shell *shell, char **tab, int i, char **new_args)
{
	t_lst	*first;
	int		y;

	first = ft_calloc(sizeof(t_lst), shell);
	first->next = NULL;
	y = ft_wildcard(shell, tab[i], first);
	if (y)
		new_args = ft_wildcard_new_tab_creation(y, first, first->file, shell);
	y = -1;
	while (tab[i][++y])
	{
		if (tab[i][y] == '*' && shell->is_quoted[i][y])
			new_args = NULL;
	}
	return (new_args);
}

int	ft_find_wildcard(t_shell *shell, char **tab)
{
	int		i;
	char	**new_args;

	i = -1;
	new_args = NULL;
	while (tab[++i])
	{
		if (find_wildcard(tab[i], shell, i))
		{
			new_args = ft_wildcard_loop(shell, tab, i, new_args);
			if (new_args)
			{
				i = ft_dup_new_tab_wildcard(shell, tab, new_args);
				tab = shell->tab;
			}
		}
	}
	return (0);
}

int	ft_create_wildcard_lst(t_lst *first, t_shell *shell, \
	struct dirent *s_dir, char *arg)
{
	int	count;

	count = 0;
	first->file = ft_strdup(s_dir->d_name, shell);
	if (!ft_is_invalid_arg_wildcard(arg, first->file) && ++count)
		ft_lstnew_wildcard(ft_strdup(first->file, shell), first, shell);
	else if (s_dir->d_type == DT_DIR)
	{
		first->file = ft_strcat(first->file, "/", shell);
		if (!ft_is_invalid_arg_wildcard(arg, first->file) && ++count)
			ft_lstnew_wildcard(ft_strdup(first->file, shell), first, shell);
	}
	return (count);
}

static int	ft_wildcard(t_shell *shell, char *arg, t_lst *first)
{
	DIR				*dir;
	struct dirent	*s_dir;
	char			*directory_path;
	int				count;

	count = 0;
	directory_path = find_wildcard_directory(shell, arg, &arg);
	first->file = directory_path;
	dir = opendir(directory_path);
	if (!dir)
		return (0);
	s_dir = readdir(dir);
	while (s_dir)
	{
		count += ft_create_wildcard_lst(first, shell, s_dir, arg);
		s_dir = readdir(dir);
	}
	first->file = directory_path;
	closedir(dir);
	return (count);
}
