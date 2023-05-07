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

void	ft_init_shell(t_shell *shell)
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
	shell->err = 0;
	shell->exit_status = 0;
	shell->i = 0;
}

int	main(int argc, char **argv, char **envp)
{
	char		*buff;
	t_shell		shell;

	if (argc == 2 && !ft_strcmp(argv[1], "--exit"))
		printf("The exit status will be shown at exit.\n");
	else if (argc != 1)
		return (write(2, "Bad args\n", 10), 1);
	shell.exit_status = 0;
	while (1)
	{
		buff = readline("Minishell :) > ");
		ft_init_shell(&shell);
		add_history(buff);
		if (tab_creation(&shell, buff) == 0 && buff && buff[0])
			ft_do_the_execve_thing(&shell, envp);
		if (argc == 2 && !ft_strcmp(argv[1], "--exit"))
			printf("		The exit status is = %d\n", shell.exit_status);
		ft_close_everything_lol(&shell);
		ft_free_everything_lol(&shell);
		exit(shell.exit_status);
		free(buff);
	}
	return (0);
}
