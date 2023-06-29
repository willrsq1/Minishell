/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_exec_no_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 00:14:46 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/29 22:35:38 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_no_pipes(t_shell *shell, char **envp);
static void	ft_waitpid_no_pipes(t_shell *shell, pid_t pid);

void	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;

	if (ft_special_operands(shell, envp) || ft_variables_substitution(shell))
		return ;
	g_exit_code = 0;
	ft_get_heredocs(shell);
	if (g_exit_code == 130 || ft_builtins(shell, shell->tab, envp) == OK)
		return ;
	pid = fork();
	if (pid == 0)
	{
		ft_get_redi(shell);
		ft_dup2_exec_no_pipes(shell);
		if (ft_builtins_in_child(shell, shell->tab, envp) == OK)
			ft_end_program(shell, OK, g_exit_code);
		get_cmd_no_pipes(shell, envp);
		write(1, "\x1b[32m", 6);
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	}
	ft_waitpid_no_pipes(shell, pid);
}

static void	get_cmd_no_pipes(t_shell *shell, char **envp)
{
	t_pipex	*pipex;

	pipex = ft_calloc(sizeof(t_pipex), shell);
	pipex->nb_cmds = 1;
	pipex->shell = shell;
	pipex->is_quoted = ft_calloc(sizeof(int **) * 2, shell);
	pipex->is_quoted[0] = shell->is_quoted;
	pipex->commands = ft_calloc(sizeof(char **) * 2, shell);
	ft_get_envp_paths(pipex, envp);
	pipex->commands[0] = shell->tab;
	if (ft_get_cmd(pipex, 0) == ERROR)
		ft_end_program(shell, OK, g_exit_code);
	shell->no_pipes_cmd = pipex->cmd;
}

void	ft_dup2_exec_no_pipes(t_shell *shell)
{
	if (shell->infile == FAIL)
		shell->infile = STDIN_FILENO;
	if (shell->outfile == FAIL)
		shell->outfile = STDOUT_FILENO;
	if (dup2(shell->infile, STDIN_FILENO) == FAIL)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	if (dup2(shell->outfile, STDOUT_FILENO) == FAIL)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
}

static void	ft_waitpid_no_pipes(t_shell *shell, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && status == 2 && write(2, "\n", 2))
		g_exit_code = SIGINT_EXITVALUE;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	ft_signal(shell);
}
