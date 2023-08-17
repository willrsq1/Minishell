/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   H_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:07:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 16:03:22 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_exit_arg_handling(char **tab, t_shell *shell, int i, int sign);
static void	ft_exit_syntax_error(char **tab, t_shell *shell);

int	ft_exit(t_shell *shell, char **envp)
{
	char	*buffer;

	buffer = ft_strdup("exit\n", shell);
	if (!shell->pipex)
		write(2, buffer, ft_strlen(buffer));
	if (shell->tab && shell->tab[1])
	{
		ft_exit_arg_handling(shell->tab, shell, 0, 1);
		if (shell->tab[2])
		{
			g_exit_code = ERROR;
			buffer = ft_strdup("Minishell: exit: \033[0;31mtoo many ", shell);
			buffer = ft_strcat(buffer, "arguments\x1b[0m \U0001F621\n", shell);
			write(2, buffer, ft_strlen(buffer));
			return (OK);
		}
	}
	ft_end_program(shell, OK, g_exit_code);
	if (envp)
		return (OK);
	return (OK);
}

long long int	ft_exit_arg_handling_error(unsigned long long result, \
	int digit, int sign, t_shell *shell)
{
	if (result * 10 + digit > LLONG_MAX || result > ULLONG_MAX / 10)
	{
		if (sign == 1)
		{
			ft_exit_syntax_error(shell->tab, shell);
		}
		else if (sign == -1)
		{
			if ((result * 10) + digit - 1 > LLONG_MAX || \
				result > ULLONG_MAX / 10)
				ft_exit_syntax_error(shell->tab, shell);
		}
	}
	return ((result * 10) + digit);
}

static void	ft_exit_arg_handling(char **tab, t_shell *shell, int i, int sign)
{
	unsigned long long	result;

	if (tab[1][0] == '-' && ++i)
		sign = -1;
	if (tab[1][0] == '+' && ++i)
		sign = 1;
	if (!(tab[1][i] >= '0' && tab[1][i] <= '9'))
		ft_exit_syntax_error(tab, shell);
	while (tab[1][i] && tab[1][i] == '0')
		i++;
	result = 0;
	while (tab[1][i])
	{
		if (!(tab[1][i] >= '0' && tab[1][i] <= '9'))
			ft_exit_syntax_error(tab, shell);
		if (result >= LLONG_MAX / 10)
		{
			result = ft_exit_arg_handling_error(result, \
				tab[1][i] - 48, sign, shell);
			break ;
		}
		result = result * 10 + (tab[1][i++] - 48);
	}
	g_exit_code = (result * sign) % 256;
}

static void	ft_exit_syntax_error(char **tab, t_shell *shell)
{
	char	*buffer;

	g_exit_code = SYNTAX_ERROR;
	buffer = ft_strdup("Minishell: exit: \033[0;31m", shell);
	buffer = ft_strcat(buffer, tab[1], shell);
	buffer = ft_strcat(buffer, \
		"\x1b[0m: numeric argument required \U0001F621\n", shell);
	write(2, buffer, ft_strlen(buffer));
	ft_end_program(shell, OK, SYNTAX_ERROR);
}
