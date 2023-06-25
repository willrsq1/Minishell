/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:36:59 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 18:24:12 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	*buf;

	buf = NULL;
	buf = getcwd(buf, PATH_MAX);
	if (!buf)
		return (perror("Minishell: "), g_exit_code = ERROR, OK);
	printf("%s\n", buf);
	g_exit_code = OK;
	free(buf);
	return (0);
}
