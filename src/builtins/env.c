/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:47:03 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 05:56:13 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	failure_env(const char *message, char **elem)
{
	int	i;

	i = 0;
	while (elem && elem[i])
		free(elem[i++]);
	if (elem)
		free(elem);
	perror(message);
	exit(EXIT_FAILURE);
}

int	lst_size_env(t_env *lst)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}

int	env_error(char **env, int output_fd)
{
	(void)output_fd;
	ft_putstr_fd("bash: export: `", 2);
	if (env[0])
	{
		ft_putstr_fd(env[0], 2);
		if (env[1])
		{
			ft_putstr_fd("=", 2);
			ft_putstr_fd(env[1], 2);
		}
	}
	ft_putstr_fd("': not a valid identifier\n", 2);
	free_tab(env);
	return (EXIT_FAILURE);
}

int	exec_env(t_cmd *cmd, int fd_out, t_env *lst_env)
{
	t_env	*tmp;

	if (!cmd || !lst_env)
		return (ft_putstr_fd("bash: env: env not defined\n", 2),
			EXIT_FAILURE);
	tmp = lst_env;
	while (tmp)
	{
		if (tmp->value != NULL)
		{
			ft_putstr_fd(tmp->name, fd_out);
			ft_putstr_fd("=", fd_out);
			ft_putendl_fd(tmp->value, fd_out);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
