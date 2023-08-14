/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   G_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:13 by wruet-su          #+#    #+#             */
/*   Updated: 2023/07/23 23:56:22 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	env builtin:
	printfs the env. */

int	ft_env(char **envp)
{
	int	i;

	i = -1;
	while (envp && envp[++i])
		printf("%s\n", envp[i]);
	g_exit_code = OK;
	return (0);
}
