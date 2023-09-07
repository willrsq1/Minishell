/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_pipex_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 17:46:32 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 17:46:32 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_size_until_pipe(t_shell *shell, int lign);

/*	Setss up the pipex struct. */

void	execution_with_pipes_initialisation(t_pipex *p)
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
	p->heredoc_fds = ft_calloc(sizeof(int) * p->nb_cmds, p->shell);
	while (++i < p->nb_cmds - 1)
	{
		p->fds[i] = ft_calloc(sizeof(int) * 2, p->shell);
		p->pipe[i] = ft_calloc(sizeof(int) * 2, p->shell);
	}
	p->fds[i] = ft_calloc(sizeof(int) * 2, p->shell);
}

/*	Divides the char **tab input into a char ***tab.
	Parses the input; put each token into its new tab;
		When a pipe is encountered, create a new tab for the tokens to come.
	Does this "big split" for is_quoted as well.*/

void	ft_split_tab_in_pipex_tabs(int nb_of_cmds, t_pipex *p, t_shell *shell)
{
	int		tab_id;
	int		i;
	int		tab_len;
	int		tab_lign;

	p->commands = ft_calloc(sizeof(char **) * (nb_of_cmds + 1), shell);
	p->is_quoted = ft_calloc(sizeof(int **) * (nb_of_cmds + 1), shell);
	tab_id = -1;
	i = -1;
	while (++tab_id < nb_of_cmds)
	{
		tab_lign = -1;
		tab_len = get_size_until_pipe(shell, i);
		p->commands[tab_id] = ft_calloc(sizeof(char *) * (tab_len + 1), shell);
		p->is_quoted[tab_id] = ft_calloc(sizeof(int *) * (tab_len + 1), shell);
		while (shell->tab[++i] && \
			ft_strcmp_unquoted(shell->tab[i], "|", shell->is_quoted[i]) != OK)
		{
			p->commands[tab_id][++tab_lign] = shell->tab[i];
			p->is_quoted[tab_id][tab_lign] = shell->is_quoted[i];
		}
	}
}

/*	Returns the number of tokens before the pipe. */

static int	get_size_until_pipe(t_shell *shell, int i)
{
	int	nb;

	nb = 0;
	while (shell->tab[++i])
	{
		if (ft_strcmp_unquoted(shell->tab[i], "|", shell->is_quoted[i]) == OK)
			break ;
		nb++;
	}
	return (nb);
}

/* Setups all the fds[x][y] to -1 (FAIL) except for those that can't be pipes.
	Gets the heredocs fds into a tab. However, the heredocs tokens won't
		be removed. But the reads(0) happend here.
	The stored fds will be accessed later, in the child processes. */

void	ft_get_heredocs_pipex(t_pipex *p, int i)
{
	t_shell	*shell;

	i = 0;
	p->fds[0][0] = STDIN_FILENO;
	p->fds[0][1] = FAIL;
	p->fds[p->nb_cmds - 1][0] = FAIL;
	p->fds[p->nb_cmds - 1][1] = STDOUT_FILENO;
	while (++i < p->nb_cmds - 1)
	{
		p->fds[i][0] = FAIL;
		p->fds[i][1] = FAIL;
	}
	i = -1;
	shell = p->shell;
	while (++i < p->nb_cmds && g_exit_code != SIGINT_EXITVALUE)
	{
		p->heredoc_fds[i] = FAIL;
		shell->tab = p->commands[i];
		shell->is_quoted = p->is_quoted[i];
		p->shell->infile = FAIL;
		ft_get_heredocs(shell);
		if (p->shell->infile != FAIL)
			p->heredoc_fds[i] = p->shell->infile;
	}
}

/*	Waits for all childs to finish.
	Gets the exit status.
	If SIGINT happens, only write one '\n'. */

void	ft_close_pipes(int i, t_pipex *p)
{
	int	y;
	int	status;
	int	sigint_received;

	y = -1;
	sigint_received = 0;
	status = 0;
	while (++y < i)
	{	
		if (p->forks_id[y] != -1)
			waitpid(p->forks_id[y], &status, 0);
		if (WIFSIGNALED(status) && status == 2)
		{
			if (sigint_received == 0 && ++sigint_received)
				write(2, "\n", 2);
			g_exit_code = SIGINT_EXITVALUE;
		}
		p->forks_id[y] = -1;
	}
	if (status == 11 && write(2, "Segmentation Fault\n", 19))
			g_exit_code = SEGFAULT_EXITVALUE;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
}
