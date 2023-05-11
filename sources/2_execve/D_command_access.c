/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_command_access.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:52:01 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 21:15:07 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_get_cmd_p2(t_pipex *p, int index);
static int	ft_get_cmd_error_check(t_pipex *p, t_shell *shell, char *cmd);

void	ft_get_envp_paths(t_pipex *p, char **envp)
{
	int		i;
	char	*envp2;

	i = 0;
	if (!envp)
	{
		p->paths = NULL;
		return ;
	}
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	envp2 = envp[i] + 5;
	p->paths = ft_split(envp2, ':', p->shell);
}

int	ft_get_cmd(t_pipex *p, int i)
{
	int	return_value;

	if (!p->commands[i] || (!p->commands[i][0] && !p->is_quoted[i][0]))
		return (-1);
	if (!p->commands[i][0] || !p->commands[i][0][0])
	{
		write(2, "'': command not found\n", 23);
		return (-1);
	}
	return_value = ft_get_cmd_p2(p, i);
	if (return_value == -1)
	{
		write(2, p->commands[i][0], ft_strlen(p->commands[i][0]));
		write(2, ": command not found\n", 21);
		return (-1);
	}
	else if (return_value < 0)
		return (-1);
	return (0);
}

static int	ft_get_cmd_p2(t_pipex *p, int index)
{
	char	*path_tested;
	char	*command;
	int		i;

	i = 42;
	if (p->commands[index][0][0] == '.' || p->commands[index][0][0] == '/')
		i = ft_get_cmd_error_check(p, p->shell, p->commands[index][0]);
	if (i != 42)
		return (i);
	i = -1;
	command = ft_strcat("/", p->commands[index][0], p->shell);
	if (p->paths)
	{
		while (p->paths[++i])
		{
			path_tested = ft_strcat(p->paths[i], command, p->shell);
			if (access(path_tested, X_OK) != -1)
				return (p->cmd = path_tested, 0);
		}
	}
	else if (access(p->commands[index][0], X_OK) == 0)
		return (p->cmd = command, 0);
	return (-1);
}

static int	ft_get_cmd_error_check(t_pipex *p, t_shell *shell, char *cmd)
{
	int	fd;

	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		write(2, "Minishell: .: filename argument required\n", 42);
		write(2, ".: usage: . filename [arguments]\n", 34);
		ft_end_program(shell, 0, 2);
	}
	fd = open(cmd, O_DIRECTORY);
	if (fd != -1)
	{
		ft_add_tbc_list(fd, shell);
		open(cmd, O_CREAT);
		perror(cmd);
		ft_end_program(shell, 0, 126);
	}
	if (access(cmd, X_OK) != -1)
		return (p->cmd = cmd, 0);
	if (cmd[0] == '.' && cmd[1] != '/')
		return (42);
	perror(ft_strcat("Minishell: ", cmd, shell));
	return (-2);
}
