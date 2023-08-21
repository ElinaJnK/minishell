/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:33:26 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 21:56:17 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*set_env(void)
{
	t_env	*lst_env;
	char	*pwd;

	lst_env = NULL;
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (exit(1), NULL);
	add_back_env(&lst_env, new_env("PWD", pwd));
	add_back_env(&lst_env, new_env("SHLVL", "1"));
	add_back_env(&lst_env, new_env("_", "/usr/bin/env"));
	return (lst_env);
}

void	init_env(char **env, t_env **lst_env, t_all **all)
{
	if (!*env)
	{
		*lst_env = set_env();
		*all = build_all(*lst_env);
		(*all)->is_env = 0;
	}
	else
	{
		*lst_env = spy_env(env);
		*all = build_all(*lst_env);
		(*all)->is_env = 1;
	}
}
