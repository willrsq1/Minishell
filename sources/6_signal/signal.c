/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 23:58:09 by wruet-su          #+#    #+#             */
/*   Updated: 2023/10/19 20:08:27 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sig_int_handler(int signum, siginfo_t *siginfo, void *context);

/*	ignores sigterm signal
	puts sig int to ignore - then puts it to the sig handler function. */

void	ft_signal(void)
{
	struct sigaction	signal_int;

	sigemptyset(&signal_int.sa_mask);
	signal_int.sa_sigaction = sig_int_handler;
	signal_int.sa_flags = 23;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	sigaction(SIGINT, &signal_int, NULL);
}

/* Puts exit code = 130; puts the prompt bach if outside of a child */

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
