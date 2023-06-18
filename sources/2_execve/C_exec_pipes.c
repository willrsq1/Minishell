/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_exec_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 00:13:01 by root              #+#    #+#             */
/*   Updated: 2023/06/17 11:41:18 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_forking(t_pipex *p, char **envp);
static void	ft_fork_loop(t_pipex *p, char **envp, int i);
static void	ft_check_for_redirections(t_pipex *p, int i);

void	ft_pipex(int argc, t_init *init, char **envp)
{
	t_pipex	*p;

	ft_pipex_big_tab(init->pipes_number, init, init->shell);
	p = ft_calloc(sizeof(t_pipex), init->shell);
	p->nb_cmds = argc + 1;
	p->shell = init->shell;
	p->shell->pipex = p;
	ft_pipex_initialisation(p);
	ft_get_envp_paths(p, envp);
	p->commands = init->args;
	p->is_quoted = init->is_quoted;
	ft_get_heredocs_pipex(p, 0);
	ft_forking(p, envp);
}

static void	ft_forking(t_pipex *p, char **envp)
{
	int	i;

	i = -1;
	while (++i < p->nb_cmds)
	{
		if (i < p->nb_cmds - 1)
			ft_pipe(p->pipe[i], p->shell);
		p->forks_id[i] = fork();
		if (p->forks_id[i] == -1)
			ft_end_program(p->shell, ERROR, EXIT_FAILURE);
		if (p->forks_id[i] == 0)
			ft_fork_loop(p, envp, i);
		if (i < p->nb_cmds - 1)
			close(p->pipe[i][1]);
	}
	ft_close_pipes(i, p);
}

static void	ft_fork_loop(t_pipex *p, char **envp, int i)
{
	ft_check_for_redirections(p, i);
	if (p->fds[i][0] == FAIL)
	{
		close(p->pipe[i - 1][1]);
		p->fds[i][0] = p->pipe[i - 1][0];
	}
	if (dup2(p->fds[i][0], 0) == FAIL)
		ft_end_program(p->shell, ERROR, EXIT_FAILURE);
	if (p->fds[i][1] == FAIL)
	{
		close(p->pipe[i][0]);
		p->fds[i][1] = p->pipe[i][1];
	}
	if (dup2(p->fds[i][1], 1) == FAIL)
		ft_end_program(p->shell, ERROR, EXIT_FAILURE);
	if (!ft_strcmp(p->cmd, "exit"))
		ft_exit(p->shell);
	execve(p->cmd, p->commands[i], envp);
	ft_end_program(p->shell, ERROR, EXIT_FAILURE);
}

static void	ft_check_for_redirections(t_pipex *p, int i)
{
	t_shell	*shell;

	shell = p->shell;
	shell->tab = p->commands[i];
	shell->is_quoted = p->is_quoted[i];
	shell->pipe_heredoc = NULL;
	shell->infile = FAIL;
	shell->outfile = FAIL;
	ft_get_redi(shell);
	if (shell->infile != FAIL)
		p->fds[i][0] = p->shell->infile;
	if (shell->infile == -2)
		p->fds[i][0] = p->heredoc_fds[i];
	if (shell->outfile != FAIL)
		p->fds[i][1] = p->shell->outfile;
	if (ft_get_cmd(p, i) == ERROR)
		ft_end_program(shell, OK, COMMAND_ERROR);
}