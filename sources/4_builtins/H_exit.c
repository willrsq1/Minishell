/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   H_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:07:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 18:24:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_exit_arg_handling(char **tab, t_shell *shell);
static void	ft_exit_syntax_error(char **tab, t_shell *shell);

int	ft_exit(t_shell *shell, char **envp)
{
	if (!shell->pipex)
		write(2, "exit\n", 6);
	g_exit_code = OK;
	if (shell->tab && shell->tab[1])
	{
		ft_exit_arg_handling(shell->tab, shell);
		if (shell->tab[2])
		{
			g_exit_code = ERROR;
			write(2, "Minishell: exit: too many arguments\n", 37);
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
	if (result * 10 + digit > LLONG_MAX)
	{
		if (sign == 1)
		{
			ft_exit_syntax_error(shell->tab, shell);
		}
		else if (sign == -1)
		{
			if ((result * 10) + digit - 1 > LLONG_MAX)
				ft_exit_syntax_error(shell->tab, shell);
		}
	}
	return ((result * 10) + digit);
}

static void	ft_exit_arg_handling(char **tab, t_shell *shell)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = -1;
	sign = 1;
	if (tab[1][0] == '-' && i++)
		sign = -1;
	if (tab[1][0] == '+' && i++)
		sign = 1;
	result = 0;
	while (tab[1][++i])
	{
		if (!(tab[1][i] >= '0' && tab[1][i] <= '9'))
			ft_exit_syntax_error(tab, shell);
		if (result >= LLONG_MAX / 10)
		{
			result = ft_exit_arg_handling_error(result, \
				tab[1][i] - 48, sign, shell);
			break ;
		}
		else
			result = result * 10 + (tab[1][i] - 48);
	}
	g_exit_code = (result * sign) % 256;
}

static void	ft_exit_syntax_error(char **tab, t_shell *shell)
{
	g_exit_code = SYNTAX_ERROR;
	write(2, "Minishell: exit: ", 18);
	write(2, tab[1], ft_strlen(tab[1]));
	write(2, ": numeric argument required\n", 29);
	ft_end_program(shell, OK, SYNTAX_ERROR);
}
