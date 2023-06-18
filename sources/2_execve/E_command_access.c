/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_command_access.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:52:01 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/01 18:03:24 by wruet-su         ###   ########.fr       */
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
	while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp || !envp[i])
	{
		p->paths = NULL;
		return ;
	}
	envp2 = envp[i] + 5;
	p->paths = ft_split(envp2, ':', p->shell);
}

int	ft_get_cmd(t_pipex *p, int i)
{
	int	return_value;

	if (!p->commands[i] || (!p->commands[i][0] && !p->is_quoted[i][0]))
		return (ERROR);
	if (!p->commands[i][0] || !p->commands[i][0][0])
	{
		write(2, "'': command not found\n", 23);
		return (ERROR);
	}
	return_value = ft_get_cmd_p2(p, i);
	if (return_value == ERROR)
	{
		write(2, \
			ft_strcat(p->commands[i][0], \
				": \033[0;31mcommand not found\033[0m\n", p->shell), \
					ft_strlen(p->commands[i][0]) + 32);
		return (ERROR);
	}
	else if (return_value < OK)
		return (ERROR);
	return (OK);
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
			if (access(path_tested, X_OK) != FAIL)
				return (p->cmd = path_tested, OK);
		}
	}
	else if (access(p->commands[index][0], X_OK) == OK)
		return (p->cmd = command, OK);
	return (ERROR);
}

static int	ft_get_cmd_error_check(t_pipex *p, t_shell *shell, char *cmd)
{
	int	fd;

	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		write(2, "Minishell: .: filename argument required\n", 42);
		write(2, ".: usage: . filename [arguments]\n", 34);
		ft_end_program(shell, OK, SYNTAX_ERROR);
	}
	fd = open(cmd, O_DIRECTORY);
	if (fd != FAIL)
	{
		ft_add_tbc_list(fd, shell);
		open(cmd, O_CREAT);
		perror(cmd);
		ft_end_program(shell, OK, DIRECTORY_ERROR);
	}
	if (access(cmd, X_OK) != FAIL)
		return (p->cmd = cmd, OK);
	if (cmd[0] == '.' && cmd[1] != '/')
		return (42);
	perror(ft_strcat("Minishell: ", cmd, shell));
	return (FAIL);
}
