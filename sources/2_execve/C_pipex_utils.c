/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 20:42:53 by marvin            #+#    #+#             */
/*   Updated: 2023/03/11 20:42:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_size_until_pipe(t_shell *shell, int lign);

void	ft_pipex_initialisation(t_pipex *p)
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

void	ft_pipex_big_tab(int pipes_number, t_init *init, t_shell *shell)
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

void	ft_get_heredocs_pipex(t_pipex *p, int i)
{
	t_shell	*shell;

	i = 0;
	p->fds[0][0] = 0;
	p->fds[0][1] = -1;
	p->fds[p->nb_cmds - 1][0] = -1;
	p->fds[p->nb_cmds - 1][1] = 1;
	while (++i < p->nb_cmds - 1)
	{
		p->fds[i][0] = -1;
		p->fds[i][1] = -1;
	}
	i = -1;
	shell = p->shell;
	while (++i < p->nb_cmds)
	{
		p->heredoc_fds[i] = -1;
		shell->tab = p->commands[i];
		shell->is_quoted = p->is_quoted[i];
		p->shell->infile = -1;
		ft_get_heredocs(shell);
		p->bad_open_heredocs[i] = shell->bad_open;
		if (p->shell->infile != -1)
			p->heredoc_fds[i] = p->shell->infile;
	}
}

void	ft_close_pipes(int i, t_pipex *p)
{
	int	y;
	int	status;

	y = -1;
	ft_close_everything_lol(p->shell);
	while (++y < i)
	{	
		if (p->forks_id[y] != -1)
			waitpid(p->forks_id[y], &status, 0);
	}
	if (WIFEXITED(status))
		p->shell->exit_status = WEXITSTATUS(status);
	else
		p->shell->exit_status = 0;
}