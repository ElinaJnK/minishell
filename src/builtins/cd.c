/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:46:54 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/24 19:07:50 by ejankovs         ###   ########.fr       */
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

char	*in_envi(char *name, t_env *lst_env)
{
	t_env	*tmp;

	tmp = lst_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	go_home(t_env **lst_env, t_all *all)
{
	char	*pwd;
	char	*res;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_putstr_fd("bash: pwd\n", 2),
			EXIT_FAILURE);
	if (!in_env("OLDPWD", pwd, *lst_env))
		add_back_env(lst_env, new_env("OLDPWD", pwd));
	free(pwd);
	res = in_envi("HOME", *lst_env);
	if (!res)
		ft_putstr_fd("bash: cd: HOME not set\n", 2);
	if (chdir(res) == -1)
	{
		if (all->is_env == 1)
			return (ft_putstr_fd("bash: cd: ", 2), ft_putstr_fd(res, 2),
				ft_putstr_fd(": No such file or directory\n", 2), EXIT_FAILURE);
		else
			return (ft_putstr_fd("bash: cd: HOME not set\n", 2), EXIT_FAILURE);
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
	if (!cmd->args[1] || !ft_strncmp(cmd->args[1], "", 1))
		return (go_home(&(all->env), all));
	if (cmd->args[2])
		return (ft_putstr_fd("bash: cd: too many arguments\n", 2), EXIT_FAILURE);
	if (cd(cmd, &(all->env), all) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
