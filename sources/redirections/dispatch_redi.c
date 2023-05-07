/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch_redi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 18:56:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 15:04:30 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_file_redirection(char *arg, int i, t_shell *shell);
static void	ft_heredoc_was_found(t_shell *shell, int i);

void	ft_get_redi(t_shell *shell)
{
	int		i;
	char	**tab;

	tab = shell->tab;
	shell->bad_open = 0;
	i = -1;
	while (tab[++i])
	{
		// printf("ARG %s\n", tab[i]);
		if (ft_file_redirection(tab[i], i, shell) == 0)
			i--;
		else if (ft_strcmp(tab[i], "<<") == 0 && \
			shell->is_quoted[i][0] == 0 && shell->is_quoted[i][1] == 0)
			ft_heredoc_was_found(shell, i--);
		else if (ft_strcmp(tab[i], "<") == 0 && shell->is_quoted[i][0] == 0)
			ft_infile(shell, i--);
		else if (ft_strcmp(tab[i], ">>") == 0 && \
			shell->is_quoted[i][0] == 0 && shell->is_quoted[i][1] == 0)
			ft_outfile_append(shell, i--);
		else if (ft_strcmp(tab[i], ">") == 0 && shell->is_quoted[i][0] == 0)
			ft_outfile_trunc(shell, i--);
		if (shell->bad_open == 1)
			return ;
	}
}

static int	ft_file_redirection(char *arg, int i, t_shell *shell)
{
	int		y;
	char	*next;

	if (arg_is_unquoted(arg, shell->is_quoted[i]) != 0 || \
		ft_find_redi_with_fd(arg, 0) == 0)
		return (-1);
	y = 0;
	next = shell->tab[i + 1];
	while (arg[y] && (arg[y] >= '0' && arg[y] <= '9'))
		y++;
	if (ft_strcmp(&arg[y], "<<") == 0)
		ft_fd_redi_heredoc(shell, i, ft_atoi_redi(arg, shell, next, 1));
	else if (ft_strcmp(&arg[y], "<") == 0)
		ft_fd_redi_infile(shell, i, ft_atoi_redi(arg, shell, next, 1));
	else if (ft_strcmp(&arg[y], ">>") == 0)
		ft_fd_redi_outfile_append(shell, i, ft_atoi_redi(arg, shell, next, 2));
	else if (ft_strcmp(&arg[y], ">") == 0)
		ft_fd_redi_outfile_trunc(shell, i, ft_atoi_redi(arg, shell, next, 3));
	return (0);
}

static void	ft_heredoc_was_found(t_shell *shell, int i)
{
	if (shell->pipex)
		shell->infile = -2;
	else
		shell->infile = shell->heredoc;
	ft_remove_redi_ligns(shell, i);
}

void	ft_remove_redi_ligns(t_shell *shell, int i)
{
	while (shell->tab[i + 2])
	{
		shell->is_quoted[i] = shell->is_quoted[i + 2];
		shell->tab[i] = ft_strdup(shell->tab[i + 2], shell);
		i++;
	}
	shell->tab[i] = NULL;
	shell->is_quoted[i] = NULL;
	shell->is_quoted[i + 1] = NULL;
	shell->tab[i + 1] = NULL;
}
