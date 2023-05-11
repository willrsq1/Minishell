/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_pipex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 00:13:01 by root              #+#    #+#             */
/*   Updated: 2023/05/11 15:59:20 by wruet-su         ###   ########.fr       */
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
			ft_end_program(p->shell, 1, errno);
		if (p->forks_id[i] == 0)
			ft_fork_loop(p, envp, i);
	}
	ft_close_pipes(i, p);
}

static void	ft_fork_loop(t_pipex *p, char **envp, int i)
{
	ft_check_for_redirections(p, i);
	if (p->fds[i][0] == -1)
	{
		close(p->pipe[i - 1][1]);
		p->fds[i][0] = p->pipe[i - 1][0];
	}
	if (dup2(p->fds[i][0], 0) < 0)
		ft_end_program(p->shell, 1, errno);
	if (p->fds[i][1] == -1)
	{
		close(p->pipe[i][0]);
		p->fds[i][1] = p->pipe[i][1];
	}
	if (dup2(p->fds[i][1], 1) < 0)
		ft_end_program(p->shell, 1, errno);
	if (!ft_strcmp(p->cmd, "exit"))
		ft_exit(p->shell);
	if (execve(p->cmd, p->commands[i], envp) == -1)
	{
		ft_free_close_perror(p->shell, "execve fail", 1);
		exit(COMMAND);
	}
}

static void	ft_check_for_redirections(t_pipex *p, int i)
{
	t_shell	*shell;

	shell = p->shell;
	shell->tab = p->commands[i];
	shell->is_quoted = p->is_quoted[i];
	shell->pipe_heredoc = NULL;
	shell->infile = -1;
	shell->outfile = -1;
	ft_get_redi(shell);
	if (shell->infile != -1)
		p->fds[i][0] = p->shell->infile;
	if (shell->infile == -2)
		p->fds[i][0] = p->heredoc_fds[i];
	if (shell->outfile != -1)
		p->fds[i][1] = p->shell->outfile;
	if (ft_get_cmd(p, i) != 0)
		ft_end_program(shell, 0, COMMAND);
}
