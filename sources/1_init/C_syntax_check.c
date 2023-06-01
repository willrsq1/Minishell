/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_syntax_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 02:33:08 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/02 00:03:24 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_check_redi_tokenss(char **tab, t_shell *shell, int i);
static int	ft_check_pipes_tokens(char **tab, t_shell *shell);
static int	ft_check_parenthesis_tokens(char **tab, t_shell *shell);
static int	no_token_after_last_pipe(int i, char *s, t_shell *shell);

int	ft_syntax_error(char **tab, t_shell *shell)
{
	if (ft_check_redi_tokenss(tab, shell, -1) == ERROR)
		return (shell->exit_status = SYNTAX_ERROR, ERROR);
	if (ft_check_pipes_tokens(tab, shell) == ERROR)
		return (shell->exit_status = SYNTAX_ERROR, ERROR);
	if (ft_check_parenthesis_tokens(tab, shell) == ERROR)
		return (shell->exit_status = SYNTAX_ERROR, ERROR);
	return (OK);
}

static int	ft_check_redi_tokenss(char **tab, t_shell *shell, int i)
{
	char	*s;

	while (tab[++i])
	{
		if (ft_is_token_redi(tab[i], shell->is_quoted[i]) || \
			(!(arg_is_unquoted(tab[i], shell->is_quoted[i]) == ERROR || \
			!ft_find_redi_with_fd(tab[i], 0))))
		{
			s = tab[i + 1];
			if (s && ft_is_token_meta(s, shell->is_quoted[i + 1]))
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `", 20);
				write(2, s, ft_strlen(s));
				return (write(2, "'\n", 3), ERROR);
			}
			if (!s)
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `newline'\n", 29);
				return (ERROR);
			}
		}
	}
	return (0);
}

static int	ft_check_pipes_tokens(char **tab, t_shell *shell)
{
	int		i;
	i = 0;
	while (tab[i])
	{
		if (ft_is_token_operand(tab[i], shell->is_quoted[i]))
		{
			if ((tab[i + 1] && ft_is_token_operand(tab[i + 1], \
				shell->is_quoted[i + 1])) || i == 0)
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `", 20);
				if (i > 0)
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "'\n", 3), ERROR);
			}
			if (!tab[i + 1])
				return (no_token_after_last_pipe(i, NULL, shell));
		}
		i++;
	}
	return (OK);
}	
	
static int	ft_check_parenthesis_tokens(char **tab, t_shell *shell)
{
	int		i;
	i = 0;
	while (tab[i])
	{
		if (tab[i][0] == '(' && !shell->is_quoted[i][0])
		{
			if (!(tab[i][ft_strlen(tab[i]) - 1] == ')' && \
				!shell->is_quoted[i][ft_strlen(tab[i])]))
			{
				write(2, "Minishell: syntax error near", 29);
				write(2, " unexpected token `", 20);
				if (i > 0)
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "'\n", 3), ERROR);
			}
		}
		//upgrade this
		i++;
	}
	return (OK);
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
