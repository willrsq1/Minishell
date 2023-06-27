/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:59 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/27 17:10:47 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	buf[FILENAME_MAX];

	getcwd(".", FILENAME_MAX);
	if (!buf[0])
		return (perror("Minishell: pwd"), g_exit_code = ERROR, OK);
	printf("%s\n", buf);
	g_exit_code = OK;
	return (0);
}
