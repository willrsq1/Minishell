/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_access.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:52:01 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 03:18:55 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_get_cmd(t_pipex *p, int index);
static int	ft_thing(t_pipex *p, t_shell *shell, char *cmd);

int	ft_command_check(t_pipex *p, int i)
{
	int	return_value;

	if (!p->commands[i] || (!p->commands[i][0] && !p->is_quoted[i][0]))
		return (-1);
	if (!p->commands[i][0] || !p->commands[i][0][0])
	{
		write(2, "'': command not found\n", 23);
		return (-1);
	}
	return_value = ft_get_cmd(p, i);
	if (return_value == -1)
	{
		p->shell->err = 127;
		write(2, p->commands[i][0], ft_strlen(p->commands[i][0]));
		write(2, ": command not found\n", 21);
		return (-1);
	}
	else if (return_value < 0)
		return (-1);
	return (0);
}

static int	ft_get_cmd(t_pipex *p, int index)
{
	char	*path_tested;
	char	*command;
	int		i;

	i = 42;
	if (p->commands[index][0][0] == '.' || p->commands[index][0][0] == '/')
		i = ft_thing(p, p->shell, p->commands[index][0]);
	if (i != 42)
		return (p->shell->err = errno, i);
	i = -1;
	if (ft_strcmp(p->commands[index][0], "exit") == 0)
		return (p->cmd = p->commands[index][0], 0);
	command = ft_strjoin_ppx("/", p->commands[index][0], p);
	if (p->paths)
	{
		while (p->paths[++i])
		{
			path_tested = ft_strjoin_ppx(p->paths[i], command, p);
			if (access(path_tested, X_OK) != -1)
				return (p->cmd = path_tested, 0);
		}
	}
	else if (access(p->commands[index][0], X_OK) == 0)
		return (p->cmd = command, 0);
	return (-1);
}

static int	ft_thing(t_pipex *p, t_shell *shell, char *cmd)
{
	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		write(2, "Minishell: .: filename argument required\n", 42);
		return (write(2, ".: usage: . filename [arguments]\n", 34), -2);
	}
	if (open(cmd, O_DIRECTORY) != -1)
	{
		open(cmd, O_CREAT);
		perror(cmd);
		return (shell->bad_open = 126, -2);
	}
	if (access(cmd, X_OK) != -1)
		return (p->cmd = cmd, 0);
	if (cmd[0] == '.' && cmd[1] != '/')
		return (42);
	write(2, "Minishell: ", 12);
	perror(cmd);
	if (shell)
		return (-2);
	return (-2);
}

int	ft_false_pipex_get_good_cmd(t_shell *shell, char **envp)
{
	t_pipex	*pipex;

	pipex = ft_calloc(sizeof(t_pipex), shell);
	if (!pipex)
		return (1);
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
	ft_get_paths_in_struct(pipex, envp);
	pipex->commands[0] = ft_split(shell->tab[0], '\0', shell);
	if (ft_command_check(pipex, 0) != 0)
		return (1);
	shell->no_pipes_cmd = pipex->cmd;
	return (0);
}
