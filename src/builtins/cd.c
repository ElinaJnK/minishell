#include "minishell.h"

int	exec_cd(t_cmd *cmd)
{
	if (!cmd)
		return (failure_exec("Error: cd doesnt exist"), EXIT_FAILURE);
	if (!cmd->args)
		return (EXIT_SUCCESS);
	if (chdir(cmd->args[1]) == -1)
		return (failure_exec("Error: cd doesnt exist"), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
