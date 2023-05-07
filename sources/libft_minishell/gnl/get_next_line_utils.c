/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 12:13:10 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/22 00:16:47 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/get_next_line.h"

size_t	ft_strlcpy_gnl(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
	{
		while (src[i])
			i++;
		return (i);
	}
	while (src[i] && i < size - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

size_t	ft_strlen_gnl(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_bzero_gnl(void *s, size_t n)
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

void	*ft_calloc_gnl(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb == SIZE_MAX && size == SIZE_MAX)
		return (NULL);
	ptr = (char *)malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero_gnl(ptr, nmemb * size);
	return (ptr);
}

char	*ft_strdup_gnl(const char *s)
{
	char	*str;
	size_t	size;

	str = (char *) s;
	size = ft_strlen_gnl(s);
	str = malloc(size + 1);
	if (str == 0)
		return (NULL);
	size = 0;
	while (s[size])
	{
		str[size] = s[size];
		size++;
	}
	str[size] = '\0';
	return (str);
}
