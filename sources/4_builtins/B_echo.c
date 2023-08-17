/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:55 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/17 19:42:35 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_echo_option_no_new_line(char **tab);
static void	ft_echo_no_option(char **tab);

/*	Echo builtin:
	If no arg, only print -n;
	If first arg == -n, launch the no_new_line part.
	Otherwise, args are present, and no options are activated: Regular launch.*/

int	ft_echo(char **tab)
{
	g_exit_code = OK;
	if (!tab[1])
	{
		printf("\n");
		return (OK);
	}
	if (ft_strcmp(tab[1], "-n") == OK)
		ft_echo_option_no_new_line(tab);
	else
		ft_echo_no_option(tab);
	return (OK);
}

/*	-n option part: prints the args without a newline at the end.
	Starts at tab[2]: tab[0] is "echo", tab[1] is "-n".
	Prints the args, followed by spaces if there is another arg after.
	IF the arg is another "-n", or "-nnnnn" or any other nb of 'n' after a '-',
	We won't print it. (that is what is checked in the second if).
	This second if will only proc if no arg has been printed. */

static void	ft_echo_option_no_new_line(char **tab)
{
	int	i;
	int	y;

	i = 2;
	y = 0;
	while (tab[i])
	{
		if (y != -42)
			y = 0;
		if (y != -42 && tab[i][0] == '-')
		{
			while (tab[i][++y] && tab[i][y] == 'n')
				;
			if (!tab[i][y])
				y = FAIL;
		}
		if (y != FAIL)
		{
			printf("%s", tab[i]);
			if (tab[i + 1])
				printf(" ");
			y = -42;
		}
		i++;
	}
}

/*	Prints the args, spaced, followed by newline. */

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
