/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:42:53 by marvin            #+#    #+#             */
/*   Updated: 2023/03/11 20:42:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_fork_loop(t_pipex *p, char **envp, int i);
static void	ft_close_pipes(int i, t_pipex *p);
static void	ft_check_for_redirections(t_pipex *p, int i);

void	ft_forking(t_pipex *p, char **envp)
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
		else if (p->forks_id[i] != 0)
		{
			if (i < p->nb_cmds - 1)
				close(p->pipe[i][1]);
			else if (i == p->nb_cmds - 1)
				close (p->pipe[i - 1][1]);
		}
		else if (p->forks_id[i] == 0)
			ft_fork_loop(p, envp, i);
	}
	ft_close_pipes(i, p);
}

static void	ft_fork_loop(t_pipex *p, char **envp, int i)
{
	ft_check_for_redirections(p, i);
	if (p->shell->bad_open != 0)
		ft_end_program(p->shell, 0, p->shell->bad_open);
	if (p->fds[i][0] == -1)
	{
		close(p->pipe[i - 1][1]);
		p->fds[i][0] = p->pipe[i - 1][0];
	}
	if (dup2(p->fds[i][0], 0) < 0)
		ft_end_program(p->shell, 1, errno);
	if (p->fds[i][1] == -1)
	{
		p->fds[i][1] = p->pipe[i][1];
		close(p->pipe[i][0]);
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

static void	ft_close_pipes(int i, t_pipex *p)
{
	int	y;
	int	status;

	y = -1;
	ft_close_everything_lol(p->shell);
	while (++y < i)
	{	
		if (p->forks_id[y] != -1)
			waitpid(p->forks_id[y], &status, 0);
	}
	if (WIFEXITED(status))
		p->shell->exit_status = WEXITSTATUS(status);
	else
		p->shell->exit_status = 0;
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
	ft_open_rdonly("/", shell);
	if (p->bad_open_heredocs[i] == 1)
	{
		shell->bad_open = 1;
		return ;
	}
	ft_get_redi(shell);
	if (shell->bad_open == 1)
		return ;
	if (shell->infile != -1)
		p->fds[i][0] = p->shell->infile;
	if (shell->infile == -2)
		p->fds[i][0] = p->heredoc_fds[i];
	if (shell->outfile != -1)
		p->fds[i][1] = p->shell->outfile;
	if (ft_command_check(p, i) != 0 && shell->bad_open != 126)
		shell->bad_open = COMMAND;
}
