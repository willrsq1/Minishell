/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 00:13:01 by root              #+#    #+#             */
/*   Updated: 2023/05/08 22:52:57 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_pipex_tab(int pipes_number, t_init *init, t_shell *shell);
static int	get_size_until_pipe(t_shell *shell, int lign);
static void	ft_pipex_initialisation(t_pipex *p);

void	ft_pipex(int argc, t_init *init, char **envp)
{
	t_pipex	*p;

	ft_pipex_tab(init->pipes_number, init, init->shell);
	p = ft_calloc(sizeof(t_pipex), init->shell);
	p->nb_cmds = argc + 1;
	p->shell = init->shell;
	p->shell->pipex = p;
	ft_pipex_initialisation(p);
	ft_get_paths_in_struct(p, envp);
	p->commands = init->args;
	p->is_quoted = init->is_quoted;
	ft_get_heredocs_pipex(p, 0);
	ft_forking(p, envp);
}

static void	ft_pipex_tab(int pipes_number, t_init *init, t_shell *shell)
{
	char	***args;
	int		tab;
	int		lign;
	int		nb;
	int		args_lign;

	args = ft_calloc(sizeof(char **) * (pipes_number + 2), shell);
	init->is_quoted = ft_calloc(sizeof(int **) * (pipes_number + 2), shell);
	tab = -1;
	lign = -1;
	while (++tab <= pipes_number)
	{
		args_lign = -1;
		nb = get_size_until_pipe(shell, lign);
		args[tab] = ft_calloc(sizeof(char *) * (nb + 1), shell);
		init->is_quoted[tab] = ft_calloc(sizeof(int *) * (nb + 1), shell);
		while (shell->tab[++lign] && \
			!(ft_strcmp(shell->tab[lign], "|") == 0 && \
			shell->is_quoted[lign][0] == 0))
		{
			args[tab][++args_lign] = ft_strdup(shell->tab[lign], shell);
			init->is_quoted[tab][args_lign] = shell->is_quoted[lign];
		}
	}
	init->args = args;
}

static int	get_size_until_pipe(t_shell *shell, int lign)
{
	int	nb;

	nb = 0;
	while (shell->tab[++lign] && \
		!(ft_strcmp(shell->tab[lign], "|") == 0 && \
		shell->is_quoted[lign][0] == 0))
		nb++;
	return (nb);
}

static void	ft_pipex_initialisation(t_pipex *p)
{
	int	i;

	i = -1;
	p->forks_id = NULL;
	p->commands = NULL;
	p->pipe = NULL;
	p->paths = NULL;
	p->forks_id = ft_calloc(sizeof(int) * p->nb_cmds, p->shell);
	p->commands = ft_calloc(sizeof(char **) * p->nb_cmds, p->shell);
	p->pipe = ft_calloc(sizeof(int *) * p->nb_cmds - 1, p->shell);
	p->fds = ft_calloc(sizeof(int *) * p->nb_cmds, p->shell);
	p->bad_open_heredocs = ft_calloc(sizeof(int) * p->nb_cmds, p->shell);
	p->heredoc_fds = ft_calloc(sizeof(int) * p->nb_cmds, p->shell);
	while (++i < p->nb_cmds - 1)
	{
		p->bad_open_heredocs[i] = 0;
		p->fds[i] = ft_calloc(sizeof(int) * 2, p->shell);
		p->pipe[i] = ft_calloc(sizeof(int) * 2, p->shell);
	}
	p->bad_open_heredocs[i] = 0;
	p->fds[i] = ft_calloc(sizeof(int) * 2, p->shell);
}
