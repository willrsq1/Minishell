#include "../../includes/minishell.h"

static int	ft_parenthesis_chars_check_error(int count);

int	ft_check_parenthesis_tokens(char **tab, t_shell *shell)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		if (tab[i][0] == '(' && !shell->is_quoted[i][0])
		{
			if (!(tab[i][ft_strlen(tab[i]) - 1] == ')' && \
				!shell->is_quoted[i][ft_strlen(tab[i])]) || \
				(i > 0 && !ft_is_token_operand(tab[i - 1], \
				shell->is_quoted[i - 1])) || \
				!ft_is_token_operand(tab[i + 1], shell->is_quoted[i + 1]))
			{
				write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
				write(2, " unexpected token `", 20);
				if (tab[i + 1])
					write(2, tab[i + 1], ft_strlen(tab[i + 1]));
				else
					write(2, tab[i], ft_strlen(tab[i]));
				return (write(2, "' \U0001F630\n", 8), SYNTAX_ERROR);
			}
		}
		i++;
	}
	return (OK);
}

int	ft_check_parenthesis_chars(char **tab, int **iq, int quote, int y)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	while (tab[++i])
	{
		y = -1;
		quote = 0;
		while (tab[i][++y])
		{
			if (tab[i][y] == '(' && !quote && !iq[i][y])
				count++;
			if (tab[i][y] == ')' && !quote && !iq[i][y])
				count--;
			if ((tab[i][y] == '\'' || tab[i][y] == '"') && !iq[i][y])
			{
				if (quote == 0)
					quote = (tab[i][y] % 2) + 1;
				else if (quote && ((tab[i][y] % 2) + 1 == quote))
					quote = 0;
			}
		}
	}
	return (ft_parenthesis_chars_check_error(count));
}

static int	ft_parenthesis_chars_check_error(int count)
{
	if (count == OK)
		return (OK);
	write(2, "Minishell: \033[0;31msyntax error\033[0m near", 40);
	write(2, " unexpected token `", 20);
	if (count > 0)
		write(2, "(", 1);
	else
		write(2, ")", 1);
	write(2, "' \U0001F630\n", 8);
	return (SYNTAX_ERROR);
}
