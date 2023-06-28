/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_print_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 09:34:25 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/28 20:46:52 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_initializing_options_2(t_shell *shell, int argc, char **argv);
static void	ft_print_help_menu(void);

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
	int	z;
	int	j;

	i = -1;
	while (operands_tab[++i])
	{
		y = -1;
		while (operands_tab[i][++y])
		{
			z = ft_strlen(operands_tab[i][y]);
			printf("NEWLINE %d of tab[%d]= %s *** ", y, i, operands_tab[i][y]);
			j = -1;
			while (++j < z)
				printf("%d", operands_is_quoted[i][y][j]);
			printf("\n");
		}
		if (operands_tab[i + 1])
			printf("\nOPERAND IS = %d\n\n", options[i]);
	}
	printf("\n\n\n DONE\n");
}

void	ft_initializing_options(t_shell *shell, int argc, char **argv)
{
	shell->show_exit_status = 0;
	shell->show_tokens = 0;
	shell->show_tokens_operands = 0;
	shell->exit_after_first_input = 0;
	while (argc > 0)
	{
		if (!ft_strcmp(argv[argc - 1], "--show-exit-status"))
		{
			shell->show_exit_status = 1;
			printf("The exit status will be shown at exit.\n");
		}
		else if (!ft_strcmp(argv[argc - 1], "--show-tokens"))
		{
			printf("The Tokens will be showned.\n");
			shell->show_tokens = 1;
		}
		else if (!ft_strcmp(argv[argc - 1], "--exit"))
		{
			printf("The args will be showned.\n");
			shell->exit_after_first_input = 1;
		}
		else
			ft_initializing_options_2(shell, argc, argv);
		argc--;
	}
}

static void	ft_initializing_options_2(t_shell *shell, int argc, char **argv)
{
	if (!ft_strcmp(argv[argc - 1], "--help"))
		ft_print_help_menu();
	else if (!ft_strcmp(argv[argc - 1], "--show-tokens-operands"))
	{
		printf("The Operands Tokens will be showned.\n");
		shell->show_tokens_operands = 1;
	}
}

static void	ft_print_help_menu(void)
{
	printf("Usage: ./minishell [options]\n\n");
	printf("Options:\n");
	printf("\t--exit: The Minishell will exit after the first input.\n");
	printf("\t--help: The Minishell will display the Help menu.\n");
	printf("\t--show-exit-status: The Minishell will display ");
	printf("the exit status of the input.\n");
	printf("\t--show-tokens: The Minishell will display the tokens obtained ");
	printf("from the input.\n");
	printf("\t--show-tokens-operands: The Minishell will display the ");
	printf("tokens obtained when operands are used.\n\n");
}
