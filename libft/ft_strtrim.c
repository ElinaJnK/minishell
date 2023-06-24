/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:44:35 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 18:44:37 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	size_t	i;

	if (!s1)
		return (NULL);
	while (set && *s1 != '\0' && ft_strchr(set, *s1))
		++s1;
	i = ft_strlen(s1);
	while (set && i > 0 && ft_strchr(set, s1[i - 1]))
		--i;
	res = (char *)malloc((i + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1, i + 1);
	return (res);
}
