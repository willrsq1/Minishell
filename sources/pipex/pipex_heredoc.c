/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 18:41:59 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/26 11:49:50 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
