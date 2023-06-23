/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 02:30:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/23 17:04:16 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_cd_no_args(void);
static void ft_cd_too_many_args_error(void);
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
		exit_true_status = ERROR;
		perror("Minishell: cd: HOME: ");
	}
}

static void ft_cd_too_many_args_error(void)
{
	write(2, "Minishell: cd: Too many arguments\n", 35);
	exit_true_status = ERROR;
}

static void	ft_cd_one_arg(char **tab, t_shell *shell)
{
	exit_true_status = OK;
	if (tab[1][0] && chdir(tab[1]) == FAIL)
	{
		perror(ft_strcat("Minishell: cd: ", tab[1], shell));
		exit_true_status = ERROR;
	}
}