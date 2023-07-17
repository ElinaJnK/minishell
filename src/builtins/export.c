#include "minishell.h"

int	exec_export(t_cmd *cmd)
{
	if (!cmd)
		return (failure_exec("Error: pwd doesnt exist"));
	//smth
	return (EXIT_SUCCESS);
}