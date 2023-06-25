/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_var_substitution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 15:18:53 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 16:39:10 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

typedef struct s_replace_chars
{
	char		*arg;
	int			*i_q;
}	t_replace_chars;

static void	ft_substitute_var_part2(t_replace_chars r, char *var, \
	char *new_arg, int *new_is_quoted);
static int	ft_get_lenght_replace_chars(char *var, char *arg, int *is_quoted);

void	ft_substitute_var(t_shell *shell, char *var, char *arg, int *is_quoted)
{
	t_replace_chars	r;
	int				len;
	char			*new_arg;
	int				*new_is_quoted;

	len = ft_get_lenght_replace_chars(var, arg, is_quoted);
	new_arg = ft_calloc(sizeof(char) * (len + 1), shell);
	new_is_quoted = ft_calloc(sizeof(int) * (len + 1), shell);
	r.arg = arg;
	r.i_q = is_quoted;
	ft_substitute_var_part2(r, var, new_arg, new_is_quoted);
	shell->tab[shell->i] = new_arg;
	shell->is_quoted[shell->i] = new_is_quoted;
}

static void	ft_substitute_var_part2(t_replace_chars r, char *var, \
	char *new_arg, int *new_is_quoted)
{
	int	i;
	int	y;
	int	len;

	i = -1;
	while (!(r.arg[++i] == '$' && r.i_q[i] != 2))
	{
		new_arg[i] = r.arg[i];
		new_is_quoted[i] = r.i_q[i];
	}
	y = i + 1;
	len = -1;
	while (var[++len])
	{
		new_arg[i] = var[len];
		new_is_quoted[i++] = 2;
	}
	while (r.arg[y] && !(r.arg[y] == '$' && r.i_q[y] != 2) && r.arg[y] != ' ')
		y++;
	while (r.arg[y])
	{
		new_arg[i] = r.arg[y];
		new_is_quoted[i++] = r.i_q[y++];
	}
}

static int	ft_get_lenght_replace_chars(char *var, char *arg, int *is_quoted)
{
	int	i;
	int	len1;
	int	len2;
	int	len3;
	int	total_lenght;

	i = 0;
	while (!(arg[i] == '$' && is_quoted[i] != 2))
		i++;
	len1 = i++;
	while (arg[i] && !(arg[i] == '$' && is_quoted[i] != 2))
		i++;
	len2 = i - len1;
	while (arg[i])
		i++;
	len3 = i - len2;
	len2 = 0;
	while (var[len2])
		len2++;
	total_lenght = len1 + len2 + len3;
	return (total_lenght);
}
