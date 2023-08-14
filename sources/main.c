/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 03:15:05 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/13 03:15:05 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_reset_shell(t_shell *shell);

int	g_exit_code;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	ft_reset_shell(&shell);
	ft_initialize_shell(&shell, argc, argv, envp);
	envp = shell.envp;
	while (1)
	{
		ft_create_prompt(&shell, envp, argv);
		if (shell.buff[0])
			ft_minishell(&shell, envp);
		ft_close_all_fds(&shell);
		ft_clear_memory(&shell);
		ft_reset_shell(&shell);
	}
	return (EXIT_SUCCESS);
}

static void	ft_reset_shell(t_shell *shell)
{
	shell->no_exit = NO_VALUE;
	shell->tab = NULL;
	shell->pipex = NULL;
	shell->infile = NO_REDI;
	shell->outfile = NO_REDI;
	shell->heredoc = NO_HEREDOC;
	shell->pipe_heredoc = NULL;
	shell->split = NULL;
	shell->is_quoted = NULL;
	shell->buff = NULL;
	shell->quote = NO_VALUE;
	shell->no_pipes_cmd = NULL;
	shell->to_be_freed_list = NULL;
	shell->tbc_list = NULL;
	shell->exit = NO_VALUE;
}
