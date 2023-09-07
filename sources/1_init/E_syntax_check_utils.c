#include "../../includes/minishell.h"

int	ft_is_token_meta(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">") || \
			!ft_strcmp(arg, "|"))
			return (ERROR);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>") || \
			!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (ERROR);
	}
	return (OK);
}

int	ft_is_token_redi(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">"))
			return (ERROR);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "<<") || !ft_strcmp(arg, ">>"))
			return (ERROR);
	}
	return (OK);
}

int	ft_is_token_redi_not_heredoc(char *arg, int *is_quoted)
{
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "<") || !ft_strcmp(arg, ">"))
			return (ERROR);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, ">>"))
			return (ERROR);
	}
	return (OK);
}

int	ft_is_token_operand(char *arg, int *is_quoted)
{
	if (!arg)
		return (ERROR);
	if (!is_quoted[0])
	{
		if (!ft_strcmp(arg, "|") || !ft_strcmp(arg, "&") || \
			!ft_strcmp(arg, ";"))
			return (ERROR);
	}
	if (!is_quoted[0] && !is_quoted[1])
	{
		if (!ft_strcmp(arg, "||") || !ft_strcmp(arg, "&&"))
			return (ERROR);
	}
	return (OK);
}
