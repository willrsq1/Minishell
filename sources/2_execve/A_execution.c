/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 14:24:40 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 16:01:41 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_count_pipes(t_shell *shell);

/*	Everything is launched from there.
	Creates the char **tab of tokens, and its int **is_quoted.
	If the creation has an issue (syntax, quotes), the execution stops.
	Resets redirections; prints tokens it the option is activated.
	Two possible paths to execution, depending on whether there are pipes. */

void	ft_minishell(t_shell *shell, char **envp)
{
	int	number_of_pipes;

	if (tab_creation(shell, shell->buff) != OK)
		return ;
	if (shell->show_tokens)
		print_tokens(shell);
	shell->infile = NO_REDI;
	shell->outfile = NO_REDI;
	number_of_pipes = ft_count_pipes(shell);
	if (number_of_pipes > 0)
		execution_with_pipes(number_of_pipes, shell, envp);
	else
		execution_no_pipes(shell, envp);
	if (shell->show_exit_status)
		printf("		\x1b[0mThe exit status is = %d\n\n", g_exit_code);
	if (shell->exit_after_first_input)
		ft_end_program(shell, OK, g_exit_code);
}

/*	If unquoted pipes are found, the execution will be done differently. */

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
