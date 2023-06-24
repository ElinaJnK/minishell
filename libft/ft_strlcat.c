/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:43:21 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 18:43:22 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	destl;
	size_t	srcl;
	size_t	i;

	destl = ft_strlen(dst);
	srcl = ft_strlen(src);
	if (size == 0 || destl > size)
		return (size + srcl);
	i = 0;
	while (src[i] && i + destl < size - 1)
	{
		dst[destl + i] = src[i];
		i++;
	}
	dst[destl + i] = '\0';
	return (destl + srcl);
}
