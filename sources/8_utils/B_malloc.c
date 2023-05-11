/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_malloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:10:57 by wruet-su          #+#    #+#             */
/*   Updated: 2023/05/11 17:51:05 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_ptr_list(t_shell *shell, void *ptr);
static void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t nmemb, t_shell *shell)
{
	void	*ptr;

	if (nmemb == SIZE_MAX)
		return (NULL);
	ptr = NULL;
	ptr = malloc(nmemb);
	if (!ptr)
	{
		write(1, "Malloc failed !!\n", 18);
		ft_free_everything_lol(shell);
		exit (1);
	}
	ft_ptr_list(shell, ptr);
	ft_bzero(ptr, nmemb);
	return (ptr);
}

static void	ft_ptr_list(t_shell *shell, void *ptr)
{
	t_to_be_freed	*to_be_freed_lst;
	t_to_be_freed	*temp;

	to_be_freed_lst = malloc(sizeof(t_to_be_freed));
	if (!to_be_freed_lst)
	{
		write(2, "Eroor calloc\n", 14);
		ft_free_everything_lol(shell);
		exit (1);
	}
	to_be_freed_lst->to_be_freed = NULL;
	to_be_freed_lst->to_be_freed = ptr;
	to_be_freed_lst->next = NULL;
	if (shell->to_be_freed_list == NULL)
	{
		shell->to_be_freed_list = to_be_freed_lst;
		return ;
	}
	temp = shell->to_be_freed_list;
	while (temp && temp->next)
		temp = temp->next;
	temp->next = to_be_freed_lst;
}

static void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}
