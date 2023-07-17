#include "minishell.h"

int	exec_cd(t_cmd *cmd)
{
	if (!cmd)
		return (failure_exec("Error: cd doesnt exist"));
	if (!cmd->args)
		return (EXIT_SUCCESS);
	if (chdir(cmd->args[0]) == -1)
		return (failure_exec("Error: cd doesnt exist"));
	return (EXIT_SUCCESS);
}
