/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   G_itoa_atoi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 16:58:46 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/01 18:27:33 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	len_nb(unsigned int nb);
static void	ft_atoi_error(int result, char *str, t_shell *shell);

char	*ft_itoa(int nb, t_shell *shell)
{
	char	*str;
	int		i;

	i = len_nb(nb);
	str = ft_calloc(i + 2, shell);
	i--;
	if (nb == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	while (nb > 0)
	{
		str[i] = 48 + (nb % 10);
		nb = nb / 10;
		i--;
	}
	return (str);
}

static int	len_nb(unsigned int nb)
{
	int	i;

	i = 0;
	while (nb > 0 && ++i)
		nb = nb / 10;
	return (i);
}

int	ft_atoi(char *str, t_shell *shell, char *file, int mode)
{
	int					i;
	unsigned long long	result;

	i = -1;
	result = 0;
	while (str[++i] && str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i] - 48);
	if (result > 1023)
	{
		if (mode == 4)
			return ((int)result);
		if (file && file[0])
			ft_open(file, shell, mode);
		ft_atoi_error(result, str, shell);
	}
	return ((int)result);
}

static void	ft_atoi_error(int result, char *str, t_shell *shell)
{
	int	i;

	dup2(result, 0);
	i = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	str[i] = '\0';
	perror(ft_strcat("Minishell: ", str, shell));
	ft_end_program(shell, OK, ERROR);
}
