/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 18:42:06 by ksadykov          #+#    #+#             */
/*   Updated: 2022/11/19 20:11:55 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_countword(char const *s, char c)
{
	int	i;
	int	cpt;

	cpt = 0;
	i = 1;
	if (s[0] && s[0] != c)
		cpt++;
	while (s[i - 1] && s[i])
	{
		if (s[i - 1] == c && s[i] != c)
			cpt++;
		i++;
	}
	return (cpt);
}

char	*ft_buildw(char const *str, char c)
{
	int		len;
	int		i;
	char	*word;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_check(char **res, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (!res[i++])
		{
			while (j < i)
				free(res[j++]);
			free(res);
			return (NULL);
		}
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		countw;
	int		i;
	int		pos;

	if (!s)
		return (NULL);
	countw = ft_countword(s, c);
	res = (char **)malloc(sizeof(char *) * (countw + 1));
	i = 0;
	if (!res)
		return (NULL);
	if (s[0] && s[0] != c)
		res[i++] = ft_buildw(s, c);
	pos = 1;
	while (s[pos - 1] && s[pos] && i < countw)
	{
		if (s[pos - 1] == c && s[pos] != c)
			res[i++] = ft_buildw(s + pos, c);
		pos++;
	}
	res[i] = 0;
	return (ft_check(res, countw));
}
