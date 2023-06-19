/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 14:24:40 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/19 02:30:46 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_count_pipes(t_shell *shell);

void	ft_do_the_execve_thing(t_shell *shell, char **envp)
{
	t_init	init;

	if (tab_creation(shell, shell->buff) == ERROR)
		return ;
	if (shell->show_tokens)
		print_tokens(shell);
	shell->infile = -1;
	shell->outfile = -1;
	init.shell = shell;
	if (ft_find_exit_status(shell) || ft_special_operands(shell, envp, -1))
		return ;
	ft_wildcards_handling(shell, shell->tab);
	init.pipes_number = ft_count_pipes(shell);
	if (init.pipes_number > 0)
		ft_pipex(init.pipes_number, &init, envp);
	else
		exec_no_pipes(shell, envp);
}

static int	ft_count_pipes(t_shell *shell)
{
	char	**tab;
	int		i;
	int		count;

	i = -1;
	tab = shell->tab;
	count = 0;
	while (tab[++i])
	{
		if (ft_strcmp_unquoted(tab[i], "|", shell->is_quoted[i]) == OK)
			count++;
	}
	return (count);
}
