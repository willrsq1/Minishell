/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   H_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:07:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/23 17:18:45 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_exit_arg_handling(char **tab);
static void	ft_exit_syntax_error(char **tab);

int	ft_exit(t_shell *shell, char **envp)
{
	int	return_value;

	if (!shell->pipex)
		write(2, "exit\n", 6);
	if (shell->tab && shell->tab[1])
	{
		return_value = ft_exit_arg_handling(shell->tab);
		if (return_value == ERROR)
		{
			exit_true_status = ERROR;
			write(2, "Minishell: exit: too many arguments\n", 37);
			return (OK);
		}
	}
	ft_free_envp(envp, shell->initial_size_of_envp);
	ft_end_program(shell, OK, exit_true_status);
	return (OK);
}

static int	ft_exit_arg_handling(char **tab)
{
	int			i;
	int			sign;
	unsigned long long	result;

	i = -1;
	sign = 1;
	if (tab[1][0] == '-' && i++)
		sign = -1;
	result = 0;
	while (tab[1][++i])
	{
		if (!(tab[1][i] >= '0' && tab[1][i] <= '9') || i > 15)
			return (ft_exit_syntax_error(tab), SYNTAX_ERROR);
		else
			result = result * 10 + (tab[1][i] - 48);
	}
	exit_true_status = (result * sign) % 256;
	if (tab[2])
		return (ERROR);
	return (OK);	
}

static void	ft_exit_syntax_error(char **tab)
{
	exit_true_status = SYNTAX_ERROR;
	write(2, "Minishell: exit: ", 18);
	write(2, tab[1], ft_strlen(tab[1]));
	write(2, ": numeric argument required\n", 29);
}
