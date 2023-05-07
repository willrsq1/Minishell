/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 13:07:00 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/06 01:34:06 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_exit(t_shell *shell)
{
	if (!shell->pipex)
		write(2, "exit\n", 6);
	ft_free_close_perror(shell, NULL, 0);
	exit (1);
}
