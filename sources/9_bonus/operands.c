/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 23:05:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/10 02:19:00 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	***ft_create_operands_tab(t_shell *shell, char **tab, int count);
static int	***ft_create_op_is_quoted(t_shell *shell, char **tab, int count);
static int	*ft_get_op_options(t_shell *shell, char **tab, int count);

/**
 * FT_OPERANDS
 * WILL EXECUTE THE INPUT AND STOP ALL OTHER EXECUTION IF OPERANDS ARE FOUND
 * CREATES AS MANY **CHAR AS THERE ARE OPERANDS
 * EXECUTE THEM ACCORDINGLY TO THE && AND || RULES
 * RETURN 1 IF OPERANDS ARE FOUD
*/

char *join_tab(char **tab, t_shell *shell)
{
	char *new;
	int i;
	i = -1;
	int y;
	int c;
	int w;
	new = ft_calloc(10000, shell);
	w = -1;
	c = 0;
	while (tab[++i])
	{
		y = -1;
		while (tab[i][++y])
		{
			if (c && !shell->is_quoted[i][y])
			{
				c = 0;
				new[++w] = '"';
			}
			if (shell->is_quoted[i][y] && c == 0 && ++c)
				new[++w] = '"';
			new[++w] = tab[i][y];
		}
		if (c && !shell->is_quoted[i][y])
		{
			c = 0;
			new[++w] = '"';
		}
		new[++w] = ' ';
	}
	return (new);
}

int	ft_operands(t_shell *shell, char **envp, int w)
{
	int		count;
	char	***operands_tab;
	int		***operands_is_quoted;
	int		*options;

	count = ft_count_operands(shell);
	if (!count)
		return (0);
	operands_tab = ft_create_operands_tab(shell, shell->tab, count);
	operands_is_quoted = ft_create_op_is_quoted(shell, shell->tab, count);
	options = ft_get_op_options(shell, shell->tab, count);
	// print_args_operands(operands_tab, operands_is_quoted, options);
	while (operands_tab[++w])
	{
		shell->tab = operands_tab[w];
		shell->is_quoted = operands_is_quoted[w];
		shell->buff = join_tab(operands_tab[w], shell);
		ft_do_the_execve_thing(shell, envp);
		ft_close_everything_lol(shell);
		if ((options[w] == OR_OPERAND && !shell->exit_status) || \
			(options[w] == AND_OPERAND && shell->exit_status))
			break ;
	}
	return (1);
}

static char	***ft_create_operands_tab(t_shell *shell, char **tab, int count)
{
	char	***op_tab;
	int		i;
	int		y;
	int		w;

	op_tab = ft_calloc(sizeof(char **) * (count + 2), shell);
	i = -1;
	y = -1;
	while (tab[++i])
	{
		w = -1;
		op_tab[++y] = ft_calloc(sizeof(char *) * \
			(1 + ft_find_op_lenght(tab, shell, i)), shell);
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i]))
			op_tab[y][++w] = tab[i++];
		if (!tab[i])
			break ;
	}
	return (op_tab);
}

static int	***ft_create_op_is_quoted(t_shell *shell, char **tab, int count)
{
	int	***op_tab;
	int	i;
	int	y;
	int	w;

	op_tab = ft_calloc(sizeof(int **) * (count + 2), shell);
	i = -1;
	y = -1;
	while (tab[++i])
	{
		w = -1;
		op_tab[++y] = ft_calloc(sizeof(int *) * \
			(1 + ft_find_op_lenght(tab, shell, i)), shell);
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i]))
			op_tab[y][++w] = shell->is_quoted[i++];
		if (!tab[i])
			break ;
	}
	return (op_tab);
}

static int	*ft_get_op_options(t_shell *shell, char **tab, int count)
{
	int		*options;
	int		i;
	int		y;

	options = ft_calloc(sizeof(int) * (count + 2), shell);
	i = -1;
	y = -1;
	while (tab[++i])
	{
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i]))
			i++;
		if (!tab[i])
			break ;
		options[++y] = ft_is_it_operand(tab[i], shell->is_quoted[i]);
	}
	return (options);
}