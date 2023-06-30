/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_main_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 11:50:19 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/30 16:47:33 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_get_cwd_in_prompt(t_shell *shell);

void	ft_create_prompt(t_shell *shell, char **envp, char **argv)
{
	char	*prompt;
	char	*cwd;

	if (ft_strcmp(argv[1], "-c") == OK)
	{
		shell->buff = argv[2];
		shell->exit_after_first_input = 1;
		return ;
	}
	prompt = ft_strdup("\x01\x1b[96m\x02Minishell d'Arbesa \x01\x1b[0m\x02", shell);
	cwd = ft_get_cwd_in_prompt(shell);
	prompt = ft_strcat(prompt, cwd, shell);
	if (!g_exit_code)
		prompt = ft_strcat(prompt, " \x01\x1b[32m\x02> \x01\x1b[m\x02", shell);
	else
		prompt = ft_strcat(prompt, " \x01\x1b[31m\x02> \x01\x1b[m\x02", shell);
	shell->buff = readline(prompt);
	if (!shell->buff)
		ft_exit(shell, envp);
	add_history(shell->buff);
}

static char	*ft_get_cwd_in_prompt(t_shell *shell)
{
	char	*cwd;
	int		i;

	cwd = ft_getenv("PWD", shell);
	if (cwd)
	{
		i = ft_strlen(cwd);
		while (--i > -1 && cwd[i] != '/')
			;
		cwd = ft_strcat("\x01\033[38;5;207m~\x02", &cwd[i], shell);
	}
	else
		cwd = ft_strdup("\x01\xF0\x9F\x98\x81\x02", shell);
	return (cwd);
}

char	**ft_new_envp(char **envp)
{
	char	**new_envp;
	int		i;
	int		y;

	new_envp = ft_calloc(sizeof(char *) * FILENAME_MAX, NULL);
	if (!new_envp)
		return (perror("Malloc fail for new_envp"), NULL);
	i = -1;
	while (envp && envp[++i])
	{
		y = -1;
		new_envp[i] = ft_calloc(sizeof(char) * FILENAME_MAX, NULL);
		if (!new_envp[i])
		{
			perror("Malloc fail for new_envp");
			while (i >= 0)
				free(new_envp[i--]);
			free(new_envp);
			return (envp);
		}
		while (envp[i][++y])
			new_envp[i][y] = envp[i][y];
	}
	return (new_envp);
}

void	ft_shlvl(char **envp)
{
	int				i;
	int				y;
	unsigned int	level;
	char			*new_lvl;

	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], "SHLVL=", 7))
		i++;
	if (!envp[i])
		return ;
	level = ft_atoi(&envp[i][6], NULL, NULL, OK);
	new_lvl = ft_itoa(level + 1, NULL);
	if (!new_lvl)
		return ;
	y = -1;
	while (new_lvl[++y])
		envp[i][6 + y] = new_lvl[y];
	envp[i][6 + y] = '\0';
	free(new_lvl);
}
