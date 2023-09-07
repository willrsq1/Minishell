#include "../../includes/minishell.h"

static void	ft_cd_special_args(t_shell *shell, char *var);
static void	ft_cd_one_arg(char **tab, t_shell *shell);
static int	ft_cd_spe_args_error_message(char *var);

int	ft_cd(t_shell *shell, char **tab)
{
	g_exit_code = OK;
	if (!tab[1])
		ft_cd_special_args(shell, "HOME");
	else if (tab[2])
	{
		write(2, "Minishell: cd: \033[0;31mToo many arguments\x1b[0m", 45);
		write(2, "\U0001F621", 5);
		g_exit_code = ERROR;
	}
	else if (ft_strcmp(tab[1], "-") == OK)
		ft_cd_special_args(shell, "OLDPWD");
	else if (ft_strcmp(tab[1], "~") == OK || ft_strcmp(tab[1], "--") == OK)
		ft_cd_special_args(shell, "HOME");
	else
		ft_cd_one_arg(tab, shell);
	return (0);
}

static void	ft_cd_one_arg(char **tab, t_shell *shell)
{
	char	*old_pwd;
	char	*new_pwd;

	old_pwd = ft_calloc(sizeof(char) * FILENAME_MAX, shell);
	new_pwd = ft_calloc(sizeof(char) * FILENAME_MAX, shell);
	if (!getcwd(old_pwd, FILENAME_MAX))
		perror("Minishell: \033[0;31mgetcwd");
	if (chdir(tab[1]) == FAIL && tab[1][0])
	{
		tab[1] = ft_strcat(tab[1], "\x1b[0m", shell);
		perror(ft_strcat("Minishell: cd: \033[0;31m", tab[1], shell));
		g_exit_code = ERROR;
		return ;
	}
	if (!getcwd(new_pwd, FILENAME_MAX))
		perror("Minishell: \033[0;31mgetcwd");
	shell->tab[1] = ft_strcat("OLDPWD=", old_pwd, shell);
	shell->tab[2] = NULL;
	ft_export(shell, shell->tab, shell->envp);
	shell->tab[1] = ft_strcat("PWD=", new_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
}

static void	ft_cd_special_args(t_shell *shell, char *var)
{
	char	old_pwd[FILENAME_MAX];
	char	new_pwd[FILENAME_MAX];
	char	*buf;

	getcwd(old_pwd, FILENAME_MAX);
	buf = ft_getenv(var, shell);
	if (!buf && ft_cd_spe_args_error_message(var))
		return ;
	if (ft_strcmp(var, "OLDPWD") == OK)
		printf("%s\n", buf);
	if (chdir(buf) == FAIL && buf[0])
	{
		buf = ft_strcat(buf, "\x1b[0m", shell);
		perror(ft_strcat("Minishell: cd: \033[0;31m", buf, shell));
		g_exit_code = ERROR;
		return ;
	}
	getcwd(new_pwd, FILENAME_MAX);
	shell->tab = ft_split("cd OLDPWD", ' ', shell);
	shell->tab[1] = ft_strcat("OLDPWD=", old_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
	shell->tab[1] = ft_strcat("PWD=", new_pwd, shell);
	ft_export(shell, shell->tab, shell->envp);
}

static int	ft_cd_spe_args_error_message(char *var)
{
	write(2, "Minishell: cd:\033[0;31m ", 23);
	write(2, var, ft_strlen(var));
	write(2, "\x1b[0m not set \U0001F621\n", 19);
	g_exit_code = ERROR;
	return (ERROR);
}

char	*ft_getenv(char *var, t_shell *shell)
{
	int		lign;
	int		i;

	lign = ft_var_get_envp_lign(shell->envp, var);
	if (lign == FAIL || shell->envp == NULL)
		return (NULL);
	i = 0;
	while (shell->envp[lign][i] && shell->envp[lign][i] != '=')
		i++;
	return (ft_strdup(&shell->envp[lign][i + 1], shell));
}
