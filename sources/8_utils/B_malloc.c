/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_malloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 08:10:57 by wruet-su          #+#    #+#             */
/*   Updated: 2023/08/18 20:11:49 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t nmemb, t_shell *shell)
{
	void	*ptr;

	if (nmemb == SIZE_MAX)
		return (NULL);
	ptr = NULL;
	ptr = malloc(nmemb);
	if (shell && !ptr)
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb);
	if (!shell)
		return (ptr);
	ft_ptr_list(shell, ptr);
	return (ptr);
}

void	ft_ptr_list(t_shell *shell, void *ptr)
{
	t_to_be_freed			*to_be_freed_lst;
	static t_to_be_freed	*temp;

	to_be_freed_lst = malloc(sizeof(t_to_be_freed));
	if (!to_be_freed_lst)
	{
		free(ptr);
		ft_end_program(shell, ERROR, EXIT_FAILURE);
	}
	to_be_freed_lst->to_be_freed = ptr;
	to_be_freed_lst->next = NULL;
	if (shell->to_be_freed_list == NULL)
	{
		temp = to_be_freed_lst;
		shell->to_be_freed_list = to_be_freed_lst;
		return ;
	}
	temp->next = to_be_freed_lst;
	temp = temp->next;
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
