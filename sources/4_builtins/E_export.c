/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:37:03 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/20 15:25:36 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_export_no_args(char **envp, t_shell *shell);
static int	ft_export_check_arg(char *arg);
static int	ft_export_var_location(char **envp, t_shell *shell, char *arg);
static int	ft_export_get_envp_lign(char **envp, char *var_name);

/*	Export builtin:
	- If no args: prints the env.
	- For each arg, checks its validity;
		Then, finds the line in char **envp where it should go.
		Finally, writes the arg to the accroding envp lign. */

int	ft_export(t_shell *shell, char **tab, char **envp)
{
	int		i;
	int		y;
	int		z;

	z = 0;
	g_exit_code = OK;
	if (!tab[1])
		return (ft_export_no_args(envp, shell), OK);
	while (tab[++z])
	{
		i = -1;
		y = -1;
		if (ft_export_check_arg(tab[z]) == ERROR)
			return (OK);
		i = ft_export_var_location(envp, shell, tab[z]);
		if (i == FAIL)
			return (OK);
		while (tab[z][++y] && y < FILENAME_MAX)
			envp[i][y] = tab[z][y];
		if (y == FILENAME_MAX)
			write(2, "Minishell: export: arg too long\n", 33);
		envp[i][y] = '\0';
	}
	return (OK);
}

/*	If no args: prints each lign of envp: inside db quotes, with
	"declare -x" before. */

static void	ft_export_no_args(char **envp, t_shell *shell)
{
	int		i;
	int		y;
	char	*temp;
	char	**new_envp;

	i = -1;
	if (!envp)
		return ;
	new_envp = ft_export_sorted_tab(shell, envp);
	while (new_envp[++i])
	{
		y = -1;
		if (new_envp[i][0] != '_')
		{
			temp = ft_calloc(ft_strlen(new_envp[i]) + 1, shell);
			printf("declare -x ");
			while (new_envp[i][++y] && new_envp[i][y] != '=')
				temp[y] = new_envp[i][y];
			printf("%s", temp);
			if (new_envp[i][y])
				printf("=\"%s\"", &new_envp[i][y + 1]);
			printf("\n");
		}
	}
}

/*	Checks for bad inputs in the exported arg/variable. */

static int	ft_export_check_arg(char *arg)
{
	int	y;

	y = -1;
	if (arg[0] == '=' || !arg[0] || (arg[0] >= '0' && arg[0] <= '9'))
	{
		write(2, "Minishell: export: `\033[0;31m", 28);
		write(2, arg, ft_strlen(arg));
		write(2, "\x1b[0m': not a valid identifier \U0001F621\n", 36);
		return (g_exit_code = ERROR, ERROR);
	}
	while (arg[++y] && arg[y] != '=')
	{
		if (arg[y] == ' ' || arg[y] == '$' || arg[y] == '-' || \
			arg[y] == '+' || arg[y] == '*' || arg[y] == '@' || \
			arg[y] == '"' || arg[y] == '\'' || arg[y] == '!' || \
			arg[y] == '|' || arg[y] == '&' || arg[y] == '\t')
		{
			write(2, "Minishell: export: `\033[0;31m", 28);
			write(2, arg, ft_strlen(arg));
			write(2, "\x1b[0m': not a valid identifier \U0001F621\n", 36);
			return (g_exit_code = ERROR, ERROR);
		}
	}
	return (OK);
}

/*	Gets the new line to be written to.
	If the line is empty, mallocs a new line. 
	If we're reached the max size of the envp_tab, returns FAIL. */

static int	ft_export_var_location(char **envp, t_shell *shell, char *arg)
{
	int		i;
	char	*var_name;

	var_name = ft_calloc(ft_strlen(arg) + 2, shell);
	i = -1;
	while (arg[++i] && arg[i] != '=')
		var_name[i] = arg[i];
	if (arg[i])
		var_name[i] = '=';
	i = ft_export_get_envp_lign(envp, var_name);
	if (i == FILENAME_MAX)
		return (write(2, "Max exports allowed reached\n", 29), FAIL);
	if (i != FAIL && envp[i] == NULL)
	{
		envp[i + 1] = NULL;
		envp[i] = malloc(sizeof(char) * FILENAME_MAX);
		if (!envp[i])
			ft_end_program(shell, ERROR, ERROR);
	}
	return (i);
}

/*	Returns the lign to put the arg to.
	If the variable doesn't exit already, returns the last line. 
	If no envp, returns -1 (FAIL).*/

static int	ft_export_get_envp_lign(char **envp, char *var_name)
{
	int	i;
	int	y;

	i = -1;
	while (envp[++i])
	{
		y = 0;
		while (var_name[y] && envp[i][y] && var_name[y] == envp[i][y])
		{
			if (var_name[y] == '=')
				return (i);
			y++;
		}
		if (var_name[y] && var_name[y] == '=' && !envp[i][y])
			return (i);
		if (!var_name[y] && envp[i][y] == '=')
			return (i);
		if (!var_name[y] && !envp[i][y])
			return (i);
	}
	return (i);
}
