#include "minishell.h"

int	exec_pwd(t_cmd *cmd)
{
	char	*pwd;

	if (!cmd)
		return (failure_exec("Error: pwd doesnt exist"));
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (EXIT_SUCCESS);
}
