/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:59 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/18 20:04:39 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*	pwd builtin.
	Prints the pwd goten from getcwd system funct. */

int	ft_pwd(t_shell *shell)
{
	char	*buf;

	buf = NULL;
	buf = ft_getenv("PWD", shell);
	if (!buf)
		return (perror("Minishell: pwd"), g_exit_code = ERROR, OK);
	printf("%s\n", buf);
	g_exit_code = OK;
	return (0);
}
