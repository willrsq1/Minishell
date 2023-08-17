/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_exec_no_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:20:07 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 19:50:21 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_no_pipes(t_shell *shell, char **envp);
static void	ft_waitpid_no_pipes(pid_t pid);

/*	Executes the input. The input doesn't have any pipes.
	STDIN and STDOUT are the initial inputs and outputs.
	First, checks for bonuses part; "&&" and "||".
	Second, substitutes the $XXX variables.
	If any of those return something other than 0, the execution stops.
	Third, gets the heredocs. Stops the exec if they were Ctrl + C'd.
	Fourth, checks for outside-of-shild builtins: exit, cd, export, unset.
	Fifth, child creation and execution: find redirections, dup2 the final fds,
	checks for in-child builtins (pwd, echo, env); if no builtins, gets the cmd.
	If execve fails, exit the child.
	In the parent, wait for the child to finish. */

void	execution_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;

	if (ft_special_operands(shell, envp) || ft_variables_substitution(shell))
		return ;
	if (ft_get_heredocs(shell) == SIGINT_EXITVALUE)
		return ;
	ft_wildcards_handling(shell, shell->tab);
	if (ft_builtins(shell, shell->tab, envp) == OK)
		return ;
	g_exit_code = OK;
	pid = fork();
	if (pid == 0)
	{
		ft_get_redi(shell);
		ft_dup2_execution_no_pipes(shell);
		if (ft_builtins_in_child(shell, shell->tab, envp) == OK)
			ft_end_program(shell, OK, g_exit_code);
		get_cmd_no_pipes(shell, envp);
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	}
	ft_waitpid_no_pipes(pid);
}

/*	Gets the PATHS from envp, looks for access to the command.
	If something goes wrong, exit the child. Otherwise, the command is ready. */

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

/*	If no redirections were found, infile and outfile are still uninitialized.
	Sets them to 0 and 1 if so.
	Dup2 the fds; if redirections were found, their fds are these variables. */

void	ft_dup2_execution_no_pipes(t_shell *shell)
{
	if (shell->infile == FAIL)
		shell->infile = STDIN_FILENO;
	if (shell->outfile == FAIL)
		shell->outfile = STDOUT_FILENO;
	if (dup2(shell->infile, STDIN_FILENO) == FAIL)
	{	
		perror(ft_strcat("dup 2: ", ft_itoa(shell->infile, shell), shell));
		ft_end_program(shell, OK, EXIT_FAILURE);
	}
	if (dup2(shell->outfile, STDOUT_FILENO) == FAIL)
	{
		perror(ft_strcat("dup 2: ", ft_itoa(shell->outfile, shell), shell));
		ft_end_program(shell, OK, EXIT_FAILURE);
	}
}

/*	Disables SIGINT, waits for the child to end.
	Gets the exit_status and renables SIGINT. */

static void	ft_waitpid_no_pipes(pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && status == 2 && write(2, "\n", 2))
		g_exit_code = SIGINT_EXITVALUE;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	ft_signal();
}
