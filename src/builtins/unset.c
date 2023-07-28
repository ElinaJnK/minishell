#include "minishell.h"

int	exec_unset(t_cmd *cmd, t_env *lst_env)
{
	t_env	*tmp;

	tmp = lst_env;
	if (!cmd || cmd->nb_args == 0)
		return (EXIT_FAILURE);
	
	while (tmp)
	{
		
	}
}
