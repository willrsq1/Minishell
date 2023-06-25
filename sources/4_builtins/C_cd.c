/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 02:30:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 18:24:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_cd_no_args(void);
static void	ft_cd_too_many_args_error(void);
static void	ft_cd_one_arg(char **tab, t_shell *shell);

int	ft_cd(t_shell *shell, char **tab)
{
	if (!tab[1])
		ft_cd_no_args();
	else if (tab[2])
		ft_cd_too_many_args_error();
	else
		ft_cd_one_arg(tab, shell);
	return (0);
}

static void	ft_cd_no_args(void)
{
	if (chdir(getenv("HOME")) == FAIL)
	{
		g_exit_code = ERROR;
		perror("Minishell: cd: HOME: ");
	}
}

static void	ft_cd_too_many_args_error(void)
{
	write(2, "Minishell: cd: Too many arguments\n", 35);
	g_exit_code = ERROR;
}

static void	ft_cd_one_arg(char **tab, t_shell *shell)
{
	g_exit_code = OK;
	if (tab[1][0] && chdir(tab[1]) == FAIL)
	{
		perror(ft_strcat("Minishell: cd: ", tab[1], shell));
		g_exit_code = ERROR;
	}
}
