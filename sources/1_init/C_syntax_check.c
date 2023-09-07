
#include "../../includes/minishell.h"

static int	ft_check_redi_tokens(char **tab, t_shell *shell, int i);
static int	ft_check_pipes_tokens(char **tab, t_shell *shell);
static int	no_token_after_last_pipe(int i, char *s, t_shell *shell);

int	ft_syntax_error(char **tab, t_shell *shell)
{
	int	return_value;

	if (ft_check_redi_tokens(tab, shell, -1) == SYNTAX_ERROR)
		return (g_exit_code = SYNTAX_ERROR, SYNTAX_ERROR);
	return_value = ft_check_pipes_tokens(tab, shell);
	if (return_value == SIGINT_EXITVALUE)
		return (g_exit_code = return_value, SIGINT_EXITVALUE);
	if (return_value != OK)
		return (g_exit_code = SYNTAX_ERROR, SYNTAX_ERROR);
	if (ft_check_parenthesis_tokens(tab, shell) == SYNTAX_ERROR)
		return (g_exit_code = SYNTAX_ERROR, SYNTAX_ERROR);
	if (ft_check_parenthesis_chars(tab, shell->is_quoted, 0, 0) == SYNTAX_ERROR)
		return (g_exit_code = SYNTAX_ERROR, SYNTAX_ERROR);
	return (OK);
}

static int	ft_check_redi_tokens(char **tab, t_shell *shell, int i)
{
	char	*s;

	while (tab[++i])
	{
		if (ft_is_token_redi(tab[i], shell->is_quoted[i]) || \
			(!(arg_is_unquoted(tab[i], shell->is_quoted[i]) == ERROR || \
			!ft_find_redi_with_fd(tab[i], 0))))
		{
			s = tab[i + 1];
			if (s && ft_is_token_meta(s, shell->is_quoted[i + 1]))
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				write(2, s, ft_strlen(s));
				return (write(2, "' \U0001F630\n", 8), SYNTAX_ERROR);
			}
			if (!s)
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `newline' \U0001F630\n", 34);
				return (SYNTAX_ERROR);
			}
		}
	}
	return (OK);
}

static int	ft_check_pipes_tokens(char **tab, t_shell *shell)
{
	int	i;

	i = -1;
	while (tab[++i])
	{
		if (ft_is_token_operand(tab[i], shell->is_quoted[i]))
		{
			if ((tab[i + 1] && ft_is_token_operand(tab[i + 1], \
				shell->is_quoted[i + 1])) || i == 0 || \
				(!ft_strcmp_unquoted(tab[i], "&", shell->is_quoted[i]) && i--))
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				if (i > 0)
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "' \U0001F630\n", 8), SYNTAX_ERROR);
			}
			if (!tab[i + 1] && ft_strcmp_unquoted(tab[i], ";", \
				shell->is_quoted[i]))
				return (no_token_after_last_pipe(i, NULL, shell));
		}
	}
	return (OK);
}

static int	no_token_after_last_pipe(int i, char *s, t_shell *shell)
{
	char	*temp;

	write(2, "> ", 3);
	temp = get_next_line(0);
	i = 0;
	if (!temp && g_exit_code == SIGINT_EXITVALUE)
		return (write(2, "\n", 2), SIGINT_EXITVALUE);
	if (!temp)
	{
		write(2, "\nMinishell: \033[0;31msyntax error\033[0m: unexpected", 48);
		write(2, " end of file \U0001F630\n", 19);
		return (SYNTAX_ERROR);
	}
	while (temp[i])
		i++;
	if (i > 0)
		temp[i - 1] = '\0';
	s = ft_strcat(shell->buff, temp, shell);
	free(temp);
	return (tab_creation(shell, s));
}
