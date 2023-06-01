/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 14:24:40 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/01 10:16:29 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_no_pipes(t_shell *shell, char **envp);
static void	get_cmd_no_pipes(t_shell *shell, char **envp);
static void	ft_dup2_exec_no_pipes(t_shell *shell);
static int	ft_count_pipes(t_shell *shell);

void	ft_do_the_execve_thing(t_shell *shell, char **envp)
{
	t_init	init;

	if (tab_creation(shell, shell->buff))
		return ;
	if (shell->show_tokens)
		print_tokens(shell);
	shell->infile = -1;
	shell->outfile = -1;
	init.shell = shell;
	if (ft_find_exit_status(shell) || ft_operands(shell, envp, -1))
		return ;
	ft_find_wildcard(shell, shell->tab);
	init.pipes_number = ft_count_pipes(shell);
	if (init.pipes_number > 0)
		ft_pipex(init.pipes_number, &init, envp);
	else
		exec_no_pipes(shell, envp);
}

static void	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;
	int		status;

	if (ft_strcmp(shell->tab[0], "exit") == 0)
		ft_exit(shell);
	pid = fork();
	if (pid == 0)
	{
		ft_get_heredocs(shell);
		ft_get_redi(shell);
		get_cmd_no_pipes(shell, envp);
		ft_dup2_exec_no_pipes(shell);
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, 1, errno);
	}
	shell->exit_status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}

static void	get_cmd_no_pipes(t_shell *shell, char **envp)
{
	t_pipex	*pipex;

	pipex = ft_calloc(sizeof(t_pipex), shell);
	pipex->nb_cmds = 1;
	pipex->shell = shell;
	pipex->forks_id = NULL;
	pipex->commands = NULL;
	pipex->pipe = NULL;
	pipex->paths = NULL;
	pipex->is_quoted = ft_calloc(sizeof(int **) * 2, shell);
	pipex->is_quoted[0] = shell->is_quoted;
	pipex->forks_id = ft_calloc(sizeof(int), shell);
	pipex->commands = ft_calloc(sizeof(char **), shell);
	ft_get_envp_paths(pipex, envp);
	pipex->commands[0] = ft_split(shell->tab[0], '\0', shell);
	if (ft_get_cmd(pipex, 0) != 0)
		ft_end_program(shell, 0, COMMAND);
	shell->no_pipes_cmd = pipex->cmd;
}

static void	ft_dup2_exec_no_pipes(t_shell *shell)
{
	if (shell->infile == -1)
		shell->infile = 0;
	if (shell->outfile == -1)
		shell->outfile = 1;
	if (dup2(shell->infile, 0) == -1)
		ft_end_program(shell, 1, errno);
	if (dup2(shell->outfile, 1) == -1)
		ft_end_program(shell, 1, errno);
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
		if (!ft_strcmp(tab[i], "|") && !shell->is_quoted[i][0])
			count++;
	}
	return (count);
}
