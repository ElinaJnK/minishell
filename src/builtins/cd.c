/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:46:54 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 03:47:30 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_cmd *cmd, t_env **lst_env, t_all *all)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_putstr_fd("bash: pwd\n", 2),
			EXIT_FAILURE);
	if (!in_env("OLDPWD", pwd, *lst_env))
		add_back_env(lst_env, new_env("OLDPWD", pwd));
	free(pwd);
	if (chdir(cmd->args[1]) == -1)
	{
		if (all->is_env == 1)
		{
			ft_putstr_fd("bash: cd: ", 2);
			ft_putstr_fd(cmd->args[1], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
		else
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
		return (EXIT_FAILURE);
	}
	pwd = getcwd(NULL, 0);
	if (!in_env("PWD", pwd, *lst_env))
		add_back_env(lst_env, new_env("PWD", pwd));
	return (free(pwd), EXIT_SUCCESS);
}

int	exec_cd(t_cmd *cmd, int output_fd, t_all *all)
{
	(void)output_fd;
	if (!cmd)
		return (ft_putstr_fd("bash: cd\n", 2),
			EXIT_FAILURE);
	if (!cmd->args || !cmd->args[1])
		return (EXIT_SUCCESS);
	if (cmd->args[2])
		return (ft_putstr_fd("bash: cd: too many arguments\n", 2), EXIT_FAILURE);
	if (cd(cmd, &(all->env), all) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
