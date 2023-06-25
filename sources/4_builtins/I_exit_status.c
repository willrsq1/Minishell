/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   I_exit_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:41:12 by wruet-su          #+#    #+#             */
/*   Updated: 2023/06/25 04:05:01 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static int	ft_dollar_sign(t_shell *shell, int i, int y)
// {
// 	char	**tab;
// 	int		**is_quoted;
// 	// char	*part1;
// 	// char	*part2;
// 	char	*new;

// 	tab = shell->tab;
// 	is_quoted = shell->is_quoted;
// 	if (tab[i][y + 1] && tab[i][y + 1] == '?' && !is_quoted[i][y + 1])
// 	{
// 		if (y == 0)
// 		{
// 			new = ft_strcat(ft_itoa(exit_true_status, shell), 				&tab[i][y + 2], shell);
// 			// printf("cc %s\n", new);
// 			shell->tab[i] = new;
// 			// if (i == 0 && !tab[1])
// 			// 	return (write(2, new, ft_strlen(new)), write(2, "\n", 2), -1);
// 			// shell->is_quoted[i] = ft_calloc(sizeof(int) * (ft_strlen(new)))
// 			// int	z;
// 			// z = -1;
// 			// while (shell->is_quoted[i][++z] != IS_QUOTED_END)
// 			// 	printf("%d\n", shell->is_quoted[i][++z]);
			
// 		}
// 	}
// 	return (y);
// }

// static int	*new_is_quoted(char *new, int *old_isq, t_shell *shell)
// {
// 	int	*new_iq;
// 	int	i;

void	ft_remove_one_token(t_shell *shell, int i)
{
	while (shell->tab[i + 1])
	{
		shell->is_quoted[i] = shell->is_quoted[i + 1];
		shell->tab[i] = ft_strdup(shell->tab[i + 1], shell);
		i++;
	}
	shell->tab[i] = NULL;
	shell->is_quoted[i] = NULL;
}

int	ft_remove_chars(char *arg, int *is_quoted)
{
	int	i;
	int	y;

	i = 1;
	if (arg[1] == '$' && is_quoted[1] != 2)
	{
		printf("PROGRAM ID COULDN'T BE FOUND !\n");
		return (arg[0] = 'I', arg[1] = 'D', 1);
	}
	while (arg[i] && (arg[i] != '$' && is_quoted[i] != 2))
		i++;
	if (!arg[i])
		return (arg[0] = '\0', is_quoted[0] = IS_QUOTED_END, 2);
	y = -1;
	while (arg[++y + i])
	{
		arg[y] = arg[y + i];
		is_quoted[y] = is_quoted[i + y];
	}
	return (arg[y] = '\0', is_quoted[y] = IS_QUOTED_END, 0);
}
	
char	*ft_get_var(t_shell *shell, char *arg, int *is_quoted)
{
	char	*var;
	int		i;

	if (!arg[1])
		return (NULL);
	i = 1;
	while (arg[i] && (arg[i] != '$' && is_quoted[i] != 2))
		i++;
	if (!arg[i])
		var = ft_strdup(&arg[1], shell);
	else
	{
		var = ft_calloc(sizeof(char) * (1 + i), shell);
		while (i-- > 1)
			var[i - 1] = arg[i];
	}
	return (var);
}

int	ft_truc_get_envp_lign(char **envp, char *var_name)
{
	int	i;
	int	y;

	i = -1;
	while (envp[++i])
	{
		y = 0;
		while (var_name[y] && envp[i][y] && var_name[y] == envp[i][y])
		{
			y++;
		}
		if (!var_name[y] && envp[i][y] == '=')
			return (i);
	}
	return (FAIL);
}

int		ft_get_lenght_replace_chars(char *var, char *arg, int *is_quoted)
{
	int	i;
	int	len1;
	int	len2;
	int	len3;
	int	total_lenght;

	i = 0;
	while (!(arg[i] == '$' && is_quoted[i] != 2))
		i++;
	len1 = i++;
	while (arg[i] && !(arg[i] == '$' && is_quoted[i] != 2))
		i++;
	len2 = i - len1;
	while (arg[i])
		i++;
	len3 = i - len2;
	len2 = 0;
	while (var[len2])
		len2++;
	total_lenght = len1 + len2 + len3;
	return (total_lenght);
}

void	ft_replace_chars(t_shell *shell, char *var, char *arg, int *is_quoted)
{
	int		len;
	int		i;
	int		y;
	char	*new_arg;
	int		*new_is_quoted;

	len = ft_get_lenght_replace_chars(var, arg, is_quoted);
	new_arg = ft_calloc(sizeof(char) * (len + 1), shell);
	new_is_quoted = ft_calloc(sizeof(int) * (len + 1), shell);
	i = -1;
	while (!(arg[++i] == '$' && is_quoted[i] != 2))
	{
		new_arg[i] = arg[i];
		new_is_quoted[i] = is_quoted[i];
	}
	// printf("NEWARG ONE = %s\n", new_arg);
	y = i + 1;
	len = -1;
	while (var[++len])
	{
		new_arg[i] = var[len];
		new_is_quoted[i++] = 2;
	}
	// printf("NEWARG TWOOO = %s\n", new_arg);
	while (arg[y] && !(arg[y] == '$' && is_quoted[y] != 2))
		y++;
	while (arg[y])
	{
		new_arg[i] = arg[y];
		new_is_quoted[i++] = is_quoted[y++];
	}
	// printf("NEWARG THREE = %s\n", new_arg);
	shell->tab[shell->i] = new_arg;
	shell->is_quoted[shell->i] = new_is_quoted;
}

int	ft_truc(t_shell *shell, char *arg, int *is_quoted, int y)
{
	char	*var;
	int		envp_lign;
	int		i;
	
	var = ft_get_var(shell, &arg[y], &is_quoted[y]);
	// printf("ARG = %s, vAR = %s\n", &arg[y], var);
	if (!var)
		return (y);
	envp_lign = ft_truc_get_envp_lign(shell->envp, var);
	if (envp_lign == FAIL)
	{
		ft_remove_chars(&arg[y], &is_quoted[y]);
		return (y - 1);
	}
	i = 0;
	while (shell->envp[envp_lign][i] != '=')
		i++;
	var = ft_strdup(&shell->envp[envp_lign][i + 1], shell);
	ft_replace_chars(shell, var, arg, is_quoted);
	return (y);
}

void	ft_dollar_question_mark(t_shell *shell, int z, int y, int len)
{
	char *arg;
	int *is_quoted;
	char	*nb;
	char	*new;
	int		*new_is_quoted;
	int		i;
	int		nb_len;

	arg = shell->tab[z];
	is_quoted = shell->is_quoted[z];
	new = ft_calloc(sizeof(char) * (len + 5), shell);
	new_is_quoted = ft_calloc(sizeof(int) * (len + 5), shell);
	i = -1;
	while (++i < y && y > 0)
	{
		new[i] = arg[i];
		new_is_quoted[i] = is_quoted[i];
	}
	y += 1;
	nb = ft_itoa(exit_true_status, shell);
	nb_len = -1;
	while (nb[++nb_len])
	{
		new[i] = nb[nb_len];
		new_is_quoted[i++] = 0;
	}
	while (arg[++y])
	{
		new[++i] = arg[y];
		new_is_quoted[i] = is_quoted[y]; 
	}
	new_is_quoted[i] = IS_QUOTED_END;
	shell->tab[z] = new;
	shell->is_quoted[z] = new_is_quoted;
}

int	ft_find_exit_status(t_shell *shell)
{
	int	i;
	int	y;

	i = -1;
	while (shell->tab[++i])
	{
		y = -1;
		while (shell->tab[i][++y])
		{
			if (shell->tab[i][y] == '$' && shell->is_quoted[i][y] != 2)
			{
				if (shell->tab[i][y + 1] == '?')
					ft_dollar_question_mark(shell, i, y, ft_strlen(shell->tab[i]));
				else
				{
					shell->i = i;
					// printf("ARG AT BEGINNING = %s\n", shell->tab[i]);
					y = ft_truc(shell, shell->tab[i], shell->is_quoted[i], y);
				}
			}
		}
		if (y == 0)
		{
			ft_remove_one_token(shell, i);
			i -= 1;
		}
		
	}
	return (0);
}

//remplacer $XXX par la var d'env
//si la var n'exite pas, supprimer $XXX et remplacer par '\0'
//si le token est mtn token[0] = '\0', supprimer le token du tableau, enlever la ligne
//appliquer tous ces changements à isquoted!