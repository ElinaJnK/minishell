#include "minishell.h"

void	failure_exec(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int	exec_echo(t_cmd *cmd, int fd_out)
{
	int	i;
	int	n_option;

	n_option = 0;
	i = 1;
	if (!cmd || !cmd->args)
		return (ft_putstr_fd("bash: echo: echo not defined\n", fd_out),
			EXIT_FAILURE);
	if (cmd->args[1] && !ft_strncmp(cmd->args[1], "-n", 3))
	{
		n_option = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], fd_out);
		if (cmd->args[i + 1] && ft_strlen(cmd->args[i + 1]))
			ft_putchar_fd(' ', fd_out);
		i++;
	}
	if (!n_option)
		ft_putchar_fd('\n', fd_out);
	return (EXIT_SUCCESS);
}

