/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:56:20 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/28 17:44:00 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_int_handler(int signum, siginfo_t *siginfo, void *context)
{
	g_exit_code = SIGINT_EXITVALUE;
	if (siginfo->si_pid == 0)
		return ;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(2, "\n", 2);
	rl_redisplay();
	if (signum && siginfo && context)
		return ;
}

void	ft_signal(t_shell *shell)
{
	struct sigaction	signal_int;

	sigemptyset(&signal_int.sa_mask);
	signal_int.sa_sigaction = sig_int_handler;
	signal_int.sa_flags = 23;
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	sigaction(SIGINT, &signal_int, NULL);
	if (shell)
		return ;
}
