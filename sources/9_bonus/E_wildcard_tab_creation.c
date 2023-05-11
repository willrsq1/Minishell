/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_tab_creation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 10:02:39 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 11:22:52 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_new_tab_filling(char **new_args, char **tab, \
	int **new_is_quoted, t_shell *shell);

char	**ft_wildc_new_init(int count, t_wildc *first, \
	char *dir_path, t_shell *shell)
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
			if (ft_strcmp(dir_path, ".") != 0)
				new[count++] = ft_strcat(dir_path, first->file, shell);
			else
				new[count++] = ft_strdup(first->file, shell);
		}
		first = first->next;
	}
	if (!new[0])
		return (NULL);
	return (new);
}

int	ft_dup_new_tab_wildcard(t_shell *shell, char **tab, char **new_args)
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
