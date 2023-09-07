#include "../../includes/minishell.h"

#ifndef NO_NEW_LINE
# define NO_NEW_LINE 1
#endif

static void	ft_echo_option_no_new_line(char **tab);
static int	ft_skip_noline_options(char **tab);
static void	ft_echo_no_option(char **tab);

/*	Echo builtin:
	If no arg, only print -n;
	If first arg == -n, launch the no_new_line part.
	Otherwise, args are present, and no options are activated: Regular launch.*/

int	ft_echo(char **tab)
{
	g_exit_code = OK;
	if (!tab[1])
	{
		printf("\n");
		return (OK);
	}
	if (tab[1][0] == '-' && tab[1][1] == 'n')
		ft_echo_option_no_new_line(tab);
	else
		ft_echo_no_option(tab);
	return (OK);
}

/*	-n option part: prints the args without a newline at the end.
	Starts at tab[2]: tab[0] is "echo", tab[1] is "-n".
	Prints the args, followed by spaces if there is another arg after.
	IF the arg is another "-n", or "-nnnnn" or any other nb of 'n' after a '-',
	We won't print it. (that is what is checked in the second if).
	This second if will only proc if no arg has been printed. */

static void	ft_echo_option_no_new_line(char **tab)
{
	int	i;
	int	y;

	i = ft_skip_noline_options(tab);
	y = NO_NEW_LINE;
	if (i == 1)
		y = 1;
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[i + 1])
			printf(" ");
		i++;
	}
	if (y != NO_NEW_LINE)
		printf("\n");
}

static int	ft_skip_noline_options(char **tab)
{
	int	i;
	int	y;

	i = 1;
	while (tab[i])
	{
		if (tab[i][0] != '-')
			return (i);
		y = 1;
		while (tab[i][y] && tab[i][y] == 'n')
			y++;
		if (tab[i][y] != '\0')
			return (i);
		i++;
	}
	return (i);
}

/*	Prints the args, spaced, followed by newline. */

static void	ft_echo_no_option(char **tab)
{
	int	i;

	i = 1;
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[++i])
			printf(" ");
	}
	printf("\n");
}
