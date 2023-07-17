#include "minishell.h"

int	exec_env(t_cmd *cmd, int fd_out)
{
	int	i;

	i = 0;
	if (!cmd)
		return (failure_exec("Error: env doesnt exist"));
	while (cmd->env[i])
	{
		ft_putendl_fd(cmd->env[i], fd_out);
		i++;
	}
	return (EXIT_SUCCESS);
}