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
	g_exit_code = 0;
	envp = ft_new_envp(envp);
	shell.envp = envp;
	ft_signal(&shell);
	ft_shlvl(envp);
	ft_initializing_options(&shell, argc, argv);
	while (1)
	{
		ft_reset_shell(&shell);
		ft_create_prompt(&shell, envp, argv);
		if (shell.buff[0])
			ft_do_the_execve_thing(&shell, envp);
		ft_close_all_fds(&shell);
		ft_clear_memory(&shell);
		if (shell.show_exit_status)
			printf("		The exit status is = %d\n", g_exit_code);
		if (shell.exit_after_first_input)
			ft_end_program(&shell, OK, g_exit_code);
	}
	return (OK);
}

static void	ft_reset_shell(t_shell *shell)
{
	shell->nb_of_double_quotes = 0;
	shell->no_exit = 0;
	shell->tab = NULL;
	shell->pipex = NULL;
	shell->infile = -1;
	shell->outfile = -1;
	shell->heredoc = -1;
	shell->pipe_heredoc = NULL;
	shell->split = NULL;
	shell->is_quoted = NULL;
	shell->buff = NULL;
	shell->quote = 0;
	shell->no_pipes_cmd = NULL;
	shell->to_be_freed_list = NULL;
	shell->tbc_list = NULL;
	shell->exit = 0;
	shell->id_pipe = 0;
	shell->i = 0;
}
