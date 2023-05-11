/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   A_execution.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 14:24:40 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 14:54:28 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	exec_no_pipes(t_shell *shell, char **envp);
static int	ft_setup_exec_no_pipes(t_shell *shell, char **envp);
static void	ft_dup2_exec_no_pipes(t_shell *shell);
static int	ft_count_pipes(t_shell *shell);

void	ft_do_the_execve_thing(t_shell *shell, char **envp)
{
	t_init	init;

	if (tab_creation(shell, shell->buff))
		return ;
	print_args(shell);
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
		shell->exit_status = exec_no_pipes(shell, envp);
}

static int	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;

	if (ft_strcmp(shell->tab[0], "exit") == 0)
		ft_exit(shell);
	pid = fork();
	if (pid == 0)
	{
		pid = ft_setup_exec_no_pipes(shell, envp);
		if (pid)
			ft_end_program(shell, 0, pid);
		ft_dup2_exec_no_pipes(shell);
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, 1, errno);
	}
	if (pid != 0)
		waitpid(pid, &pid, 0);
	if (WIFEXITED(pid))
		return (WEXITSTATUS(pid));
	return (0);
}

static int	ft_setup_exec_no_pipes(t_shell *shell, char **envp)
{
	ft_get_heredocs(shell);
	if (shell->bad_open == 1)
		return (1);
	ft_get_redi(shell);
	if (shell->bad_open == 1)
		return (1);
	if (get_cmd_no_pipes(shell, envp) != 0)
	{
		if (shell->bad_open == 126)
			return (126);
		return (COMMAND);
	}
	return (0);
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