/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   white.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 18:27:58 by ejankovs          #+#    #+#             */
/*   Updated: 2023/08/24 19:00:26 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}
