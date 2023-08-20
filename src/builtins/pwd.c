/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:50:18 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 03:50:18 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pwd(t_cmd *cmd, int output_fd)
{
	char	*pwd;

	if (!cmd)
		return (ft_putstr_fd("bash: pwd: pwd doesnt exist\n", output_fd),
			EXIT_FAILURE);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_putstr_fd("bash: pwd: pwd doesnt exist\n", output_fd),
			EXIT_FAILURE);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (EXIT_SUCCESS);
}
