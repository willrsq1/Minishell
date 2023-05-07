/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 02:33:08 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 14:19:16 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_check_pipes_tokens(char **tab, t_shell *shell);
static int	no_token_after_last_pipe(int i, char *s, t_shell *shell);
static int	ft_check_redi_tokens(char **tab, t_shell *shell);

int	ft_syntax_error(char **tab, t_shell *shell)
{
	if (ft_check_pipes_tokens(tab, shell) == 1)
		return (shell->exit_status = 2, 1);
	if (ft_check_redi_tokens(tab, shell) == 1)
		return (shell->exit_status = 2, 1);
	return (0);
}

static int	ft_check_meta_operand(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "|"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{ 
		if (!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (1);
	}
	return (0);
}

static int	ft_check_pipes_tokens(char **tab, t_shell *shell)
{
	int		i;
	char	*s;

	s = NULL;
	i = 0;
	while (tab[i])
	{
		if (ft_check_meta_operand(tab[i], shell->is_quoted[i]))
		{
			if (tab[i + 1] && ft_check_meta_operand(tab[i + 1], shell->is_quoted[i + 1]))
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `|'\n", 23);
				return (1);
			}
			if (!tab[i + 1])
				return (no_token_after_last_pipe(i, s, shell));
		}
		i++;
	}
	return (0);
}

static int	no_token_after_last_pipe(int i, char *s, t_shell *shell)
{
	char	*temp;

	write(2, "> ", 3);
	temp = get_next_line(0);
	i = 0;
	while (temp[i])
		i++;
	if (i > 0)
		temp[i - 1] = '\0';
	s = ft_strcat(shell->buff, temp, shell);
	free(temp);
	return (tab_creation(shell, s));
}

static int	ft_check_meta_redi(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">") || \
			!ft_strcmp(arg, "|"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{ 
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>") || \
			!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (1);
	}
	return (0);
}

static int	ft_check_redi(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">"))
			return (1);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{ 
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>"))
			return (1);
	}
	return (0);
}

static int	ft_check_redi_tokens(char **tab, t_shell *shell)
{
	int		i;
	char	*s;

	i = -1;
	while (tab[++i])
	{
		if (ft_check_redi(tab[i], shell->is_quoted[i]))
		{
			s = tab[i + 1];
			if (s && ft_check_meta_redi(s, shell->is_quoted[i]))
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `", 20);
				write(2, s, ft_strlen(s));
				write(2, "'\n", 3);
				return (1);
			}
			if (!s)
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `newline'\n", 29);
				return (1);
			}
		}
	}
	return (0);
}
