/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_exec_no_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 00:14:46 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/28 03:56:03 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cmd_no_pipes(t_shell *shell, char **envp);
// static int	ft_check_syntax(char **tab, t_shell *shell);
// static int	ft_check_parenthsesis_chars(char *arg, int *iq);

void	exec_no_pipes(t_shell *shell, char **envp)
{
	pid_t	pid;
	int		status;

	if (ft_special_operands(shell, envp, -1))
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
		execve(shell->no_pipes_cmd, shell->tab, envp);
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	}
	signal(SIGINT, SIG_IGN);
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && write(2, "\n", 2))
		g_exit_code = SIGINT_EXITVALUE;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	ft_signal(shell);
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

// static int	ft_check_syntax(char **tab, t_shell *shell)
// {
// 	int	**iq;
// 	int	i;

// 	i = -1;
// 	iq = shell->is_quoted;
// 	while (tab[++i])
// 	{
// 		if (ft_check_parenthsesis_chars(tab[i], iq[i]) == ERROR)
// 		{
// 			g_exit_code = SYNTAX_ERROR;
// 			return (ERROR);
// 		}
// 	}
// 	return (OK);
// }

// static int	ft_check_parenthsesis_chars(char *arg, int *iq)
// {
// 	int	y;
// 	int	check;
// 	int	check2;
// 	int	end;

// 	y = -1;
// 	end = ft_strlen(arg) - 1;
// 	check = 0;
// 	check2 = 0;
// 	if (!arg[0])
// 		return (OK);
// 	while (arg[++y])
// 	{
// 		if ((arg[y] == '(' && iq[y] == 0 && check == 1) ||
// 			(arg[end - y] == ')' && !iq[end - y] && check2))
// 		{
// 			write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
// 			write(2, " unexpected token `", 20);
// 			write(2, &arg[y], 1);
// 			return (write(2, "'\n", 3), ERROR);
// 		}
// 		if (!iq[y] && arg[y] != '(')
// 			check = 1;
// 		if (!iq[end - y] && arg[end - y] != ')')
// 			check2 = 1;
// 	}
// 	return (OK);
// }
