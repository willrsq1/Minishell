#include "../../includes/minishell.h"

static int	ft_is_a_printable_envp_arg(char *s);

/*	env builtin:
	printfs the env. */

int	ft_env(char **envp)
{
	int	i;

	i = -1;
	while (envp && envp[++i])
	{
		if (ft_is_a_printable_envp_arg(envp[i]) == OK)
			printf("%s\n", envp[i]);
	}
	g_exit_code = OK;
	return (0);
}

static int	ft_is_a_printable_envp_arg(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '=')
			return (OK);
		i++;
	}
	return (ERROR);
}
