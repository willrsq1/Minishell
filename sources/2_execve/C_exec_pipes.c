/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_exec_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 00:13:01 by root              #+#    #+#             */
/*   Updated: 2023/08/20 16:13:20 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_forking(t_pipex *p, char **envp);
static void	ft_fork_loop(t_pipex *p, char **envp, int i);
static void	ft_check_for_redirections(t_pipex *p, int i);
static void	ft_dup2_exec_pipes(t_pipex *p, int i);

void	execution_with_pipes(int number_of_pipes, t_shell *shell, char **envp)
{
	t_pipex	*p;

	p = ft_calloc(sizeof(t_pipex), shell);
	p->nb_cmds = number_of_pipes + 1;
	shell->pipex = p;
	p->shell = shell;
	execution_with_pipes_initialisation(p);
	ft_split_tab_in_pipex_tabs(p->nb_cmds, p, shell);
	ft_get_envp_paths(p, envp);
	g_exit_code = 0;
	ft_get_heredocs_pipex(p, 0);
	if (g_exit_code == SIGINT_EXITVALUE)
		return ;
	ft_forking(p, envp);
}

static void	ft_forking(t_pipex *p, char **envp)
{
	int	i;

	i = -1;
	while (++i < p->nb_cmds && g_exit_code != SIGINT_EXITVALUE)
	{
		if (i < p->nb_cmds - 1)
			ft_pipe(p->pipe[i], p->shell);
		p->forks_id[i] = fork();
		if (p->forks_id[i] == FAIL)
			ft_end_program(p->shell, ERROR, EXIT_FAILURE);
		if (p->forks_id[i] == CHILD)
			ft_fork_loop(p, envp, i);
		if (i < p->nb_cmds - 1 && reading(p, i))
			close(p->pipe[i][1]);
	}
	signal(SIGINT, SIG_IGN);
	ft_close_all_fds(p->shell);
	ft_close_pipes(i, p);
	ft_signal();
}

static void	ft_fork_loop(t_pipex *p, char **envp, int i)
{
	p->shell->tab = p->commands[i];
	p->shell->is_quoted = p->is_quoted[i];
	ft_dup2_exec_pipes(p, i);
	if (ft_special_operands(p->shell, envp) || \
		ft_variables_substitution(p->shell))
		ft_end_program(p->shell, OK, g_exit_code);
	ft_wildcards_handling(p->shell, p->commands[i]);
	p->commands[i] = p->shell->tab;
	p->is_quoted[i] = p->shell->is_quoted;
	ft_check_for_redirections(p, i);
	ft_dup2_exec_pipes(p, i);
	if (ft_builtins_in_child(p->shell, p->commands[i], envp) == OK || \
		ft_builtins(p->shell, p->commands[i], envp) == OK || \
		ft_get_cmd(p, i) == ERROR)
		ft_end_program(p->shell, OK, g_exit_code);
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
	if (shell->infile == HEREDOC_IN_PIPES)
		p->fds[i][0] = p->heredoc_fds[i];
	if (shell->outfile != FAIL)
		p->fds[i][1] = p->shell->outfile;
}

static void	ft_dup2_exec_pipes(t_pipex *p, int i)
{
	if (p->fds[i][STDIN_FILENO] == FAIL)
	{
		close(p->pipe[i - 1][1]);
		p->fds[i][STDIN_FILENO] = p->pipe[i - 1][0];
	}
	if (p->fds[i][STDOUT_FILENO] == FAIL)
	{
		close(p->pipe[i][0]);
		p->fds[i][STDOUT_FILENO] = p->pipe[i][1];
	}
	if (dup2(p->fds[i][STDIN_FILENO], STDIN_FILENO) == FAIL)
	{
		perror(ft_strcat("dup 2: ", ft_itoa(p->fds[i][0], p->shell), p->shell));
		ft_end_program(p->shell, OK, EXIT_FAILURE);
	}
	if (dup2(p->fds[i][STDOUT_FILENO], STDOUT_FILENO) == FAIL)
	{
		perror(ft_strcat("dup 2: ", ft_itoa(p->fds[i][1], p->shell), p->shell));
		ft_end_program(p->shell, ERROR, EXIT_FAILURE);
	}
}
