/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:42:48 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 18:42:50 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*src;

	if (!s)
		return (0);
	src = (unsigned char *)s;
	while (*src)
	{
		if (*src == (unsigned char)c)
			return ((char *)src);
		src++;
	}
	if (*src == (unsigned char)c)
		return ((char *)src);
	return (0);
}
