/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   G_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:13 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/22 16:41:06 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(char **envp)
{
	int	i;

	i = -1;
	while (envp && envp[++i])
		printf("%s\n", envp[i]);
	exit_true_status = OK;
	return (0);
}
