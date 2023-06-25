/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:55 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 10:29:33 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_echo_option_no_new_line(char **tab);
static void	ft_echo_no_option(char **tab);

int	ft_echo(char **tab)
{
	if (!tab[1])
	{
		printf("\n");
		return (OK);
	}
	if (ft_strcmp(tab[1], "-n") == OK)
		ft_echo_option_no_new_line(tab);
	else
		ft_echo_no_option(tab);
	exit_true_status = OK;
	return (0);
}

static void	ft_echo_option_no_new_line(char **tab)
{
	int	i;

	i = 2;
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[++i])
			printf(" ");
	}
}

static void	ft_echo_no_option(char **tab)
{
	int	i;

	i = 1;
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[++i])
			printf(" ");
	}
	printf("\n");
}
