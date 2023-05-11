/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 09:34:25 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/07 09:46:27 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_args(t_shell *shell)
{
	int	y;
	int	i;

	i = 0;
	while (shell->tab[i])
	{
		printf("Lign %d is : %s *** ", i, shell->tab[i]);
		y = -1;
		while (shell->tab[i][++y])
			printf("%d", shell->is_quoted[i][y]);
		printf("\n");
		i++;
	}
	printf("\n\n\n");
}

void	print_args_operands(char ***operands_tab, int ***operands_is_quoted, \
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
