/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:47:12 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 03:49:03 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*exit_status(void)
{
	static int	exit_s = 0;

	return (&exit_s);
}

int	is_num(char *str)
{
	int	i;

	i = 0;
	if (*str == '-')
		i++;
	if (str[i] == 0)
		return (EXIT_FAILURE);
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			break ;
		i++;
	}
	while (str[i])
	{
		if (str[i] != ' ')
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	do_end(t_all *all)
{
	free_all(all);
	exit(*exit_status());
}

int	exec_exit(t_cmd *cmd, t_all *all)
{
	if (!cmd->args)
		return (EXIT_FAILURE);
	if (cmd->n_pipes == 0)
		ft_putstr_fd("exit\n", 2);
	if (cmd->args[1])
	{
		if (is_num(cmd->args[1]) == EXIT_SUCCESS)
		{
			*exit_status() = ft_atoi(cmd->args[1]);
		}
		if (is_num(cmd->args[1]) == EXIT_FAILURE)
		{
			*exit_status() = 2;
			ft_putstr_fd("bash: exit: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
		}
		else if (cmd->nb_args >= 2)
		{
			*exit_status() = EXIT_FAILURE;
			ft_putstr_fd("bash: exit: too many arguments\n", 2);
			return (EXIT_FAILURE);
		}
	}
	return (do_end(all), EXIT_SUCCESS);
}
