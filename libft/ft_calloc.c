/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:37:43 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 18:37:46 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*t;
	size_t	s;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	if (nmemb == SIZE_MAX || size == SIZE_MAX)
		return (NULL);
	s = size * nmemb;
	t = (void *)malloc(s);
	if (!t)
		return (NULL);
	ft_bzero(t, s);
	return (t);
}
