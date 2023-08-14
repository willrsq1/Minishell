/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_print_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 09:34:25 by wruet-su          #+#    #+#             */
/*   Updated: 2023/07/23 23:44:37 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_initializing_options(t_shell *shell, int argc, char **argv);
static void	ft_initializing_options_2(t_shell *shell, int argc, char **argv);

void	ft_initialize_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	g_exit_code = 0;
	envp = ft_new_envp(envp);
	shell->envp = envp;
	ft_signal();
	ft_shlvl(envp);
	shell->show_exit_status = 0;
	shell->show_tokens = 0;
	shell->show_tokens_operands = 0;
	shell->exit_after_first_input = 0;
	shell->enable_semicolons = 0;
	ft_initializing_options(shell, argc, argv);
}

static void	ft_initializing_options(t_shell *shell, int argc, char **argv)
{
	while (--argc > 0)
	{
		if (!ft_strcmp(argv[argc], "--show-exit-status") && \
			!shell->show_exit_status++)
			printf("The exit status will be shown at exit.\n");
		else if (!ft_strcmp(argv[argc], "--show-tokens") && \
			!shell->show_tokens++)
			printf("The Tokens will be showned.\n");
		else if (!ft_strcmp(argv[argc], "--exit") && \
			!shell->exit_after_first_input++)
			printf("The args will be showned.\n");
		else if (!ft_strcmp(argv[argc], "--show-all") && \
			printf("Exit status and all tokens will be showned.\n"))
		{
			shell->show_exit_status = 1;
			shell->show_tokens = 1;
			shell->show_tokens_operands = 1;
		}
		else
			ft_initializing_options_2(shell, argc, argv);
	}
}

static void	ft_initializing_options_2(t_shell *shell, int argc, char **argv)
{
	if (!ft_strcmp(argv[argc], "--help"))
	{
		printf("Usage: ./minishell [options]\n\n");
		printf("Options:\n");
		printf("\t--exit: The Minishell will exit after the first input.\n");
		printf("\t--help: The Minishell will display the Help menu.\n");
		printf("\t--enable-semicolons: The semicolons will be enabled.\n");
		printf("\t--show-all: Exit status and all tokens will be showned.\n");
		printf("\t--show-exit-status: The Minishell will display ");
		printf("the exit status of the input.\n");
		printf("\t--show-tokens: The Minishell will display the tokens ");
		printf("obtained from the input.\n");
		printf("\t--show-tokens-operands: The Minishell will display the ");
		printf("tokens obtained when operands are used.\n\n");
	}
	else if (!ft_strcmp(argv[argc], "--show-tokens-operands") && \
		!shell->show_tokens_operands++)
		printf("The Operands Tokens will be showned.\n");
	else if (!ft_strcmp(argv[argc], "--enable-semicolons") && \
		!shell->enable_semicolons++)
		printf("The semicolons will be unabled.\n");
}

void	print_tokens(t_shell *shell)
{
	int	y;
	int	i;

	i = 0;
	printf("\n\tTokens: \n");
	while (shell->tab[i])
	{
		printf("Token %d is : %s *** ", i + 1, shell->tab[i]);
		y = -1;
		while (shell->tab[i][++y])
			printf("%d", shell->is_quoted[i][y]);
		printf("\n");
		i++;
	}
	printf("\n");
}

void	print_tokens_operands(char ***operands_tab, int ***operands_is_quoted, \
	int *options)
{
	int	y;
	int	i;
	int	j;

	i = -1;
	printf("----Start of operands tokens-----\n\n");
	while (operands_tab[++i])
	{
		y = -1;
		while (operands_tab[i][++y])
		{
			printf("Tab[%d][%d]: %s *** ", i, y, operands_tab[i][y]);
			j = -1;
			while (++j < ft_strlen(operands_tab[i][y]))
				printf("%d", operands_is_quoted[i][y][j]);
		}
		if (operands_tab[i + 1])
		{
			if ((options[i] == 401 && printf("\n\tOperand: '||'\n")) || \
				(options[i] == 402 && printf("\n\tOperand: '&&'\n")) || \
				(options[i] == 403 && printf("\n\tOperand: ';'\n")))
				continue ;
		}
	}
	printf("\n\n----End of operands tokens-----\n");
}
