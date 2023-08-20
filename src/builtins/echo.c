/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 03:46:58 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 03:48:49 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	failure_exec(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int	write_echo(t_cmd *cmd, int fd_out, int i)
{
	char	*arg;
	int		bytes_written;

	arg = cmd->args[i];
	if (arg)
	{
		bytes_written = write(fd_out, arg, ft_strlen(arg));
		if (bytes_written < 0)
		{
			ft_putstr_fd("bash: echo: write error: No space left on device\n",
				STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', fd_out);
	}
	return (EXIT_SUCCESS);
}

int	is_n(char *str)
{
	int	i;

	i = 0;
	if (!ft_strncmp(str + i, "-n", 2))
	{
		i = 2;
		while (str[i])
		{
			if (str[i] != 'n')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int	exec_echo(t_cmd *cmd, int fd_out)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	if (fd_out < 0)
		return (ft_putstr_fd("bash: echo: write error\n", 2), EXIT_FAILURE);
	if (!cmd || !cmd->args)
		return (ft_putstr_fd("bash: echo: echo not defined\n", STDERR_FILENO),
			EXIT_FAILURE);
	while (cmd->args[i] && is_n(cmd->args[i]))
		i++;
	if (i > 1)
		n_option = 1;
	while (cmd->args[i])
	{
		if (ft_strlen(cmd->args[i]))
			if (write_echo(cmd, fd_out, i) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		i++;
	}
	if (!n_option)
		ft_putchar_fd('\n', fd_out);
	return (EXIT_SUCCESS);
}
