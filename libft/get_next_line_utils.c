/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 17:29:46 by ksadykov          #+#    #+#             */
/*   Updated: 2023/04/10 17:29:46 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strdel(char **as)
{
	free(*as);
	*as = NULL;
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1l;
	size_t	s2l;
	char	*join;

	s1l = ft_strlen(s1);
	s2l = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (s1l + s2l + 1));
	if (join)
	{
		ft_strncpy(join, s1, s1l);
		ft_strncpy(join + s1l, s2, s1l + s2l);
	}
	return (join);
}
