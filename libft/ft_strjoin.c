/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:43:10 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 18:43:14 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1l;
	size_t	s2l;
	char	*join;

	if (!s1 || !s2)
		return (NULL);
	s1l = ft_strlen(s1);
	s2l = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (s1l + s2l + 1));
	if (join)
	{
		ft_memcpy(join, s1, s1l);
		ft_memcpy(join + s1l, s2, s2l);
		join[s1l + s2l] = '\0';
	}
	return (join);
}
