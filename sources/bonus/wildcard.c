/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 02:17:31 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 01:42:14 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_wildcard(t_shell *shell, char *arg, t_lst *first);
static char	**ft_wildcard_new_tab_creation(int count, t_lst *first, \
	char *directory_path, t_shell *shell);
static int	ft_new_tab_wildcard(t_shell *shell, char **tab, char **new_args);
static int	ft_new_tab_filling(char **new_args, char **tab, \
	int **new_is_quoted, t_shell *shell);

int	ft_find_wildcard(t_shell *shell, char **tab)
{
	int		i;
	char	**new_args;
	int		y;
	t_lst	*first;

	i = -1;
	while (tab[++i])
	{
		if (find_wildcard(tab[i], shell, i))
		{
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
			if (new_args)
			{
				i = ft_new_tab_wildcard(shell, tab, new_args);
				tab = shell->tab;
			}
		}
	}
	return (0);
}

static int	ft_wildcard(t_shell *shell, char *arg, t_lst *first)
{
	DIR				*dir;
	struct dirent	*s_dir;
	char			*directory_path;
	int				count;

	count = 0;
	directory_path = find_wildcard_directory(shell, arg, &arg);
	printf("Path = %s\n", directory_path);
	first->file = directory_path;
	dir = opendir(directory_path);
	if (!dir)
		return (0);
	s_dir = readdir(dir);
	while (s_dir)
	{
		first->file = ft_strdup(s_dir->d_name, shell);
		printf("ARG = %s\n", first->file);
		if (!ft_is_invalid_arg_wildcard(arg, first->file) && ++count)
			ft_lstnew_wildcard(ft_strdup(first->file, shell), first, shell);
		else if (s_dir->d_type == DT_DIR)
		{
			first->file = ft_strcat(first->file, "/", shell);
			if (!ft_is_invalid_arg_wildcard(arg, first->file) && ++count)
				ft_lstnew_wildcard(ft_strdup(first->file, shell), first, shell);
		}
		s_dir = readdir(dir);
	}
	first->file = directory_path;
	closedir(dir);
	return (count);
}

static char	**ft_wildcard_new_tab_creation(int count, t_lst *first, \
	char *directory_path, t_shell *shell)
{
	char	**new;

	new = ft_calloc(sizeof(char *) * (count + 1), shell);
	new[count] = NULL;
	count = 0;
	first = first->next;
	while (first)
	{
		if (first->file[0] != '.')
		{
			if (ft_strcmp(directory_path, ".") != 0)
				new[count++] = ft_strcat(directory_path, first->file, shell);
			else
				new[count++] = ft_strdup(first->file, shell);
		}
		first = first->next;
	}
	if (!new[0])
		return (NULL);
	return (new);
}

static int	ft_new_tab_wildcard(t_shell *shell, char **tab, char **new_args)
{
	int		i;
	int		y;
	int		new_pos;
	int		**new_is_quoted;

	i = 0;
	y = 0;
	while (tab[i])
		i++;
	while (new_args && new_args[y])
		y++;
	shell->tab = ft_calloc(sizeof(char *) * (i + y), shell);
	new_is_quoted = ft_calloc(sizeof(int *) * (i + y), shell);
	i = -1;
	y = -1;
	while (tab[++i] && !(find_wildcard(tab[i], shell, i)))
	{
		new_is_quoted[i] = shell->is_quoted[i];
		shell->tab[i] = tab[i];
	}
	shell->i = i;
	new_pos = ft_new_tab_filling(new_args, tab, new_is_quoted, shell);
	return (new_pos);
}

static int	ft_new_tab_filling(char **new_args, char **tab, \
	int **new_is_quoted, t_shell *shell)
{
	int	y;
	int	count;
	int	new_pos;
	int	i;

	y = -1;
	i = shell->i;
	while (new_args[++y])
	{
		count = -1;
		new_pos = ft_strlen(new_args[y]);
		new_is_quoted[i + y] = ft_calloc(sizeof(int) * (new_pos + 1), shell);
		while (++count < new_pos)
			new_is_quoted[i + y][count] = 0;
		shell->tab[i + y] = new_args[y];
	}
	new_pos = i + y - 1;
	while (tab[++i])
	{
		shell->tab[i + y - 1] = tab[i];
		new_is_quoted[i + y - 1] = shell->is_quoted[i];
	}
	shell->is_quoted = new_is_quoted;
	return (new_pos);
}
