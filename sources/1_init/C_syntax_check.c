/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_syntax_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 02:33:08 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/28 03:55:06 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_check_redi_tokens(char **tab, t_shell *shell, int i);
static int	ft_check_pipes_tokens(char **tab, t_shell *shell);
static int	ft_check_parenthesis_tokens(char **tab, t_shell *shell);
static int	no_token_after_last_pipe(int i, char *s, t_shell *shell);

void	ft_parenthesis_chars_check_error(int count)
{
	write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
	write(2, " unexpected token `", 20);
	if (count > 0)
		write(2, "(", 1);
	else
		write(2, ")", 1);
	write(2, "'\n", 3);
}

int	ft_check_parenthesis_chars(char **tab, int **iq, int quote, int y)
{
	int	i;
	int	count;
	
	count = 0;
	i = -1;
	while (tab[++i])
	{
		y = -1;
		quote = 0;
		while (tab[i][++y])
		{
			if (tab[i][y] == '(' && !quote && !iq[i][y])
				count++;
			if (tab[i][y] == ')' && !quote && !iq[i][y])
				count--;
			if ((tab[i][y] == '\'' || tab[i][y] == '"') && !iq[i][y])
			{
				if (quote == 0)
					quote = (tab[i][y] % 2) + 1;
				else if (quote && ((tab[i][y] % 2) + 1 == quote))
					quote = 0;
			}
		}
	}
	if (count != 0)
		return (ft_parenthesis_chars_check_error(count), ERROR);
	return (OK);
}

int	ft_syntax_error(char **tab, t_shell *shell)
{
	int	return_value;

	if (ft_check_redi_tokens(tab, shell, -1) == ERROR)
		return (g_exit_code = SYNTAX_ERROR, ERROR);
	return_value = ft_check_pipes_tokens(tab, shell);
	if (return_value == ERROR)
		return (g_exit_code = SYNTAX_ERROR, ERROR);
	if (return_value == SIGINT_EXITVALUE)
		return (g_exit_code = SIGINT_EXITVALUE, ERROR);
	if (ft_check_parenthesis_tokens(tab, shell) == ERROR)
		return (g_exit_code = SYNTAX_ERROR, ERROR);
	if (ft_check_parenthesis_chars(tab, shell->is_quoted, 0, 0) == ERROR)
		return (g_exit_code = SYNTAX_ERROR, ERROR);
	return (OK);
}

static int	ft_check_redi_tokens(char **tab, t_shell *shell, int i)
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
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				write(2, s, ft_strlen(s));
				return (write(2, "'\n", 3), ERROR);
			}
			if (!s)
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
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
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				if (i > 0)
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "'\n", 3), ERROR);
			}
			if (!tab[i + 1] && ft_strcmp_unquoted(tab[i], ";", \
				shell->is_quoted[i]))
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
				!shell->is_quoted[i][ft_strlen(tab[i])]) || \
				(i > 0 && !ft_is_token_operand(tab[i - 1], shell->is_quoted[i - 1])) || \
				!ft_is_token_operand(tab[i + 1], shell->is_quoted[i + 1]))
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				if (tab[i + 1])
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "'\n", 3), ERROR);
			}
		}
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
	if (!temp)
		return (SIGINT_EXITVALUE);
	while (temp[i])
		i++;
	if (i > 0)
		temp[i - 1] = '\0';
	s = ft_strcat(shell->buff, temp, shell);
	free(temp);
	return (tab_creation(shell, s));
}
