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

static void	ft_init_shell(t_shell *shell);

int	main(int argc, char **argv, char **envp)
{
	char		*buff;
	t_shell		shell;

	ft_initializing_options(&shell, argc, argv);
	while (1)
	{
		buff = readline("Minishell d'Arbesa > ");
		ft_init_shell(&shell);
		add_history(buff);
		shell.buff = buff;
		if (buff && buff[0])
			ft_do_the_execve_thing(&shell, envp);
		ft_close_everything_lol(&shell);
		ft_free_everything_lol(&shell);
		if (shell.show_exit_status)
			printf("		The exit status is = %d\n", shell.exit_status);
		if (shell.exit_after_first_input)
			exit(shell.exit_status);
		free(buff);
	}
	return (OK);
}

static void	ft_init_shell(t_shell *shell)
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
	shell->exit_status = 0;
	shell->i = 0;
}
