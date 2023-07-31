#include "minishell.h"

int	exec_cd(t_cmd *cmd, int output_fd)
{
	if (!cmd)
		return (ft_putstr_fd("bash: cd: cd doesnt exist\n", output_fd), EXIT_FAILURE);
	if (!cmd->args)
		return (EXIT_SUCCESS);
	if (chdir(cmd->args[1]) == -1)
		return (ft_putstr_fd("bash: cd: cd doesnt exist\n", output_fd), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
