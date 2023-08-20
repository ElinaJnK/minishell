/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:50:27 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 03:50:27 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_unset(t_cmd *cmd, t_env **lst_env)
{
	t_env	*tmp;
	int		i;

	i = 1;
	tmp = *lst_env;
	if (!cmd)
		return (EXIT_FAILURE);
	if (cmd->nb_args == 0)
		return (EXIT_SUCCESS);
	while (cmd->args[i])
	{
		tmp = *lst_env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->name, cmd->args[i], ft_strlen(tmp->name) + 1))
			{
				lst_del_env(lst_env, tmp->name);
				break ;
			}
			tmp = tmp->next;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
