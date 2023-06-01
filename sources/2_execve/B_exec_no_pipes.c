/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_exec_no_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 00:14:46 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/02 00:14:52 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_no_pipes(t_shell *shell, char **envp);
static void	ft_dup2_exec_no_pipes(t_shell *shell);

void	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;
	int		status;

	if (ft_strcmp(shell->tab[0], "exit") == OK)
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
	shell->exit_status = OK;
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
	if (ft_get_cmd(pipex, 0) == ERROR)
		ft_end_program(shell, 0, COMMAND_ERROR);
	shell->no_pipes_cmd = pipex->cmd;
}

static void	ft_dup2_exec_no_pipes(t_shell *shell)
{
	if (shell->infile == FAIL)
		shell->infile = STDIN_FILENO;
	if (shell->outfile == FAIL)
		shell->outfile = STDOUT_FILENO;
	if (dup2(shell->infile, 0) == FAIL)
		ft_end_program(shell, 1, errno);
	if (dup2(shell->outfile, 1) == FAIL)
		ft_end_program(shell, 1, errno);
}
