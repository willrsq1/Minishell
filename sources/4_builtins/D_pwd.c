/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:59 by wruet-su          #+#    #+#             */
/*   Updated: 2023/07/23 23:57:22 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	pwd builtin.
	Prints the pwd goten from getcwd system funct. */

int	ft_pwd(void)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, FILENAME_MAX);
	if (!buf)
		return (perror("Minishell: pwd:"), g_exit_code = ERROR, OK);
	printf("%s\n", buf);
	free(buf);
	g_exit_code = OK;
	return (0);
}
