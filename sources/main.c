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
static char	**ft_new_envp(t_shell *shell, char **envp);

int	exit_true_status;

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	
	ft_reset_shell(&shell);
	envp = ft_new_envp(&shell, envp);
	shell.envp = envp;
	ft_signal(&shell);
	exit_true_status = 0;
	ft_shlvl(envp);
	ft_initializing_options(&shell, argc, argv);
	shell.exit_after_first_input = 1;
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
			ft_end_program(&shell, OK, exit_true_status);
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
	while (envp && envp[i] && ft_strncmp(envp[i], "SHLVL=", 7))
		i++;
	if (!envp[i])
		return ;
	level = ft_atoi(&envp[i][6], NULL, NULL, OK);
	new_lvl = ft_itoa(level + 1, NULL);
	if (!new_lvl)
		return ;
	y = -1;
	while (new_lvl[++y])
		envp[i][6 + y] = new_lvl[y];
	envp[i][6 + y] = '\0';
	free(new_lvl);
}

static char	**ft_new_envp(t_shell *shell, char **envp)
{
	char	**new_envp;
	int		i;

	new_envp = calloc(sizeof(char *) * 1024, 1);
	i = -1;
	while (envp && envp[++i] && shell)
		new_envp[i] = strdup(envp[i]);
	return (new_envp);
}