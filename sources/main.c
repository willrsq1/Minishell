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
static void	ft_create_prompt(t_shell *shell, char **envp);
static void	ft_shlvl(char **envp);

int	exit_true_status;

void	ft_free_envp(char **envp, int i)
{
	while (envp[i])
	{
		free(envp[i]);
		envp[i++] = NULL;
	}
}

void	get_envp_size(char **envp, t_shell *shell)
{
	int	i;

	i = -1;
	while (envp[++i])
		;
	shell->initial_size_of_envp = i;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	
	ft_reset_shell(&shell);
	get_envp_size(envp, &shell);
	ft_signal(&shell);
	exit_true_status = 0;
	ft_shlvl(envp);
	ft_initializing_options(&shell, argc, argv);
	while (1)
	{
		ft_reset_shell(&shell);
		ft_create_prompt(&shell, envp);
		if (shell.buff[0])
			ft_do_the_execve_thing(&shell, envp);
		ft_close_all_fds(&shell);
		ft_clear_memory(&shell);
		if (shell.show_exit_status)
			printf("		The exit status is = %d\n", exit_true_status);
		if (shell.exit_after_first_input)
			exit(exit_true_status);
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

static void	ft_create_prompt(t_shell *shell, char **envp)
{
	shell->buff = readline("Minishell d'Arbesa > ");
	if (!shell->buff)
		ft_exit(shell, envp);
	add_history(shell->buff);
}

static void	ft_shlvl(char **envp)
{
	int				i;
	int				y;
	unsigned int	level;
	char			*new_lvl;

	i = 0;
	while (envp && ft_strncmp(envp[i], "SHLVL=", 7))
		i++;
	if (!envp[i])
		return ;
	level = ft_atoi(&envp[i][6], NULL, NULL, OK);
	new_lvl = ft_itoa(level + 1, NULL);
	if (!new_lvl)
		exit(EXIT_FAILURE);
	y = -1;
	while (new_lvl[++y])
		envp[i][6 + y] = new_lvl[y];
	envp[i][6 + y] = '\0';
	free(new_lvl);
}
