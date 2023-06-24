/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_exec_no_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 00:14:46 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/24 02:41:21 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_no_pipes(t_shell *shell, char **envp);

void	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;
	int		status;

	if (ft_special_operands(shell, envp, -1))
		return ;
	ft_get_heredocs(shell);
	if (ft_builtins(shell, shell->tab, envp) == OK)
		return ;
	if (exit_true_status == 130)
		return ;
	pid = fork();
	if (pid == 0)
	{
		ft_get_redi(shell);
		ft_dup2_exec_no_pipes(shell);
		if (ft_builtins_in_child(shell, shell->tab, envp) == OK)
			ft_end_program(shell, OK, exit_true_status);
		get_cmd_no_pipes(shell, envp);
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	}
	status = 0;
	if (waitpid(pid, &status, WUNTRACED) == -1)
	{
		printf("EXIT CHILD FAIL WAITPID\n");
		exit_true_status = SIGINT_EXITVALUE;
	}
	else if (WIFEXITED(status))
		exit_true_status = WEXITSTATUS(status);
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
	pipex->commands[0] = ft_split(shell->tab[0], '\0', shell);
	if (ft_get_cmd(pipex, 0) == ERROR)
		ft_end_program(shell, OK, COMMAND_ERROR);
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
