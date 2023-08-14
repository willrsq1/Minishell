/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_operands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 23:05:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/07/26 21:46:36 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	***ft_create_operands_tab(t_shell *shell, char **tab, int count);
static int	***ft_create_op_is_quoted(t_shell *shell, char **tab, int count);
static int	*ft_get_op_options(t_shell *shell, char **tab, int count);
static int	ft_special_operands_actions(int *options, int w);

/**
 * ft_special_operands
 * WILL EXECUTE THE INPUT AND STOP ALL OTHER EXECUTION IF OPERANDS ARE FOUND
 * CREATES AS MANY **CHAR AS THERE ARE OPERANDS
 * EXECUTE THEM ACCORDINGLY TO THE && AND || RULES
 * RETURN 1 IF OPERANDS ARE FOUD
*/

int	ft_special_operands(t_shell *shell, char **envp)
{
	int		count;
	char	***operands_tab;
	int		***operands_is_quoted;
	int		*options;
	int		w;

	w = -1;
	count = ft_count_operands(shell);
	if (!count)
		return (OK);
	operands_tab = ft_create_operands_tab(shell, shell->tab, count);
	operands_is_quoted = ft_create_op_is_quoted(shell, shell->tab, count);
	options = ft_get_op_options(shell, shell->tab, count);
	if (shell->show_tokens_operands)
		print_tokens_operands(operands_tab, operands_is_quoted, options);
	while (w > -10 && operands_tab[++w])
	{
		shell->tab = operands_tab[w];
		shell->is_quoted = operands_is_quoted[w];
		shell->buff = ft_join_tab(operands_tab[w], shell);
		ft_minishell(shell, envp);
		ft_close_all_fds(shell);
		w = ft_special_operands_actions(options, w);
	}
	return (ERROR);
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
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i], shell))
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
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i], shell))
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
		while (tab[i] && !ft_is_it_operand(tab[i], shell->is_quoted[i], shell))
			i++;
		if (!tab[i])
			break ;
		options[++y] = ft_is_it_operand(tab[i], shell->is_quoted[i], shell);
	}
	return (options);
}

static int	ft_special_operands_actions(int *options, int w)
{
	if (options[w] == OR_OPERAND && !g_exit_code)
	{
		while (options[w] == OR_OPERAND && options[w + 1])
			w++;
	}	
	else if (options[w] == AND_OPERAND && g_exit_code)
	{
		while (options[w] == AND_OPERAND && options[w + 1])
			w++;
	}
	if (options[w] == AND_OPERAND && g_exit_code)
		return (IS_QUOTED_END);
	if (options[w] == OR_OPERAND && !g_exit_code)
		return (IS_QUOTED_END);
	if (g_exit_code == SIGINT_EXITVALUE)
		return (IS_QUOTED_END);
	return (w);
}
