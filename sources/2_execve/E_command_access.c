/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_command_access.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 12:52:01 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 17:00:08 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_get_cmd_p2(t_pipex *p, int index);
static int	ft_get_cmd_err_check(t_pipex *p, t_shell *shell, char *cmd);
static void	ft_get_cmd_error_check_special_cases(t_shell *shell, char *cmd);

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
	int		return_value;
	char	*buff;

	if (!p->commands[i] || !p->commands[i][0])
		ft_end_program(p->shell, OK, OK);
	if (!p->commands[i][0][0])
	{
		buff = ft_strdup(\
			"\x1b[0m'': \033[0;31mcommand not found \U0001F621\x1b[0m\n", \
			p->shell);
		write(2, buff, ft_strlen(buff));
		return (g_exit_code = COMMAND_ERROR, ERROR);
	}
	return_value = ft_get_cmd_p2(p, i);
	if (return_value == ERROR)
	{
		buff = ft_strcat(p->commands[i][0], \
			": \033[0;31mcommand not found \U0001F621\x1b[0m\n", p->shell);
		buff = ft_strcat("\x1b[0m", buff, p->shell);
		write(2, buff, ft_strlen(buff));
		return (g_exit_code = COMMAND_ERROR, ERROR);
	}
	return (OK);
}

static int	ft_get_cmd_p2(t_pipex *p, int index)
{
	char	*path_tested;
	char	*command;
	int		i;

	i = 42;
	if (p->commands[index][0][0] == '.' || p->commands[index][0][0] == '/')
		i = ft_get_cmd_err_check(p, p->shell, p->commands[index][0]);
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

static int	ft_get_cmd_err_check(t_pipex *p, t_shell *shell, char *cmd)
{
	char	*buff;

	if (cmd[0] == '.' && cmd[1] == '\0')
	{
		buff = ft_strdup("Minishell: .: filename argument required\n", shell);
		buff = ft_strcat(buff, ".: usage: . filename [arguments]\n", shell);
		write(2, buff, ft_strlen(buff));
		ft_end_program(shell, OK, SYNTAX_ERROR);
	}
	ft_get_cmd_error_check_special_cases(shell, cmd);
	if (access(cmd, X_OK) != FAIL)
		return (p->cmd = cmd, OK);
	if (cmd[0] == '.' && cmd[1] != '/')
		return (42);
	if (access(cmd, F_OK) == FAIL)
	{
		cmd = ft_strcat(cmd, "\033[0;31m", shell);
		perror(ft_strcat("Minishell: ", cmd, shell));
		ft_end_program(shell, OK, COMMAND_ERROR);
	}
	access(cmd, X_OK);
	return (ft_end_program(shell, ERROR, PERMISSION_ERROR), FAIL);
}

static void	ft_get_cmd_error_check_special_cases(t_shell *shell, char *cmd)
{
	int		fd;
	char	*buff;

	if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '\0')
	{
		buff = ft_strcat("..", ": \033[0;31mcommand not found \U0001F621\n", \
			shell);
		write(2, buff, ft_strlen(buff));
		ft_end_program(shell, OK, COMMAND_ERROR);
	}
	fd = open(cmd, O_DIRECTORY);
	if (fd != FAIL || (cmd[0] == '.' && access(cmd, X_OK) == FAIL))
	{
		ft_add_tbc_list(fd, shell);
		open(cmd, O_CREAT);
		cmd = ft_strcat(cmd, "\x1b[0m", shell);
		perror(ft_strcat("Minishell: \033[0;31m", cmd, shell));
		ft_end_program(shell, OK, DIRECTORY_ERROR);
	}
}
