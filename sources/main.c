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
static void	ft_create_prompt(t_shell *shell);

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;

	ft_initializing_options(&shell, argc, argv);
	while (1)
	{
		ft_reset_shell(&shell);
		ft_create_prompt(&shell);
		if (shell.buff && shell.buff[0])
			ft_do_the_execve_thing(&shell, envp);
		ft_close_all_fds(&shell);
		ft_clear_memory(&shell);
		if (shell.show_exit_status)
			printf("		The exit status is = %d\n", shell.exit_status);
		if (shell.exit_after_first_input)
			exit(shell.exit_status);
	}
	return (OK);
}

static void	ft_reset_shell(t_shell *shell)
{
	shell->nb_of_double_quotes = 0;
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

static void	ft_create_prompt(t_shell *shell)
{
	// char	path[1024];

	// if (getcwd(path, sizeof(path)) != NULL)
	// {
	// 	write(2, "\033[1;36m(", 9);
	// 	write(2, path, ft_strlen(path));
	// }
	// if (shell->exit_status)
	// 	write(2, "\033[1;31m>\033[0m ", 14);
	// else
	// {
	// 	write(2, "\033[1;32m", 8);
	// 	write(2, ">", 1);
	// 	write(2, "\033[0m ", 6);
	// }
	shell->exit_status = 0;
	shell->buff = readline("Minishell d'Arbesa > ");
	add_history(shell->buff);
}
