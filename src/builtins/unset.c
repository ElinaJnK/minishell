#include "minishell.h"

int	exec_unset(t_cmd *cmd, t_env *lst_env)
{
	t_env	*tmp;
	int		i;

	i = 1;
	tmp = lst_env;
	if (!cmd || cmd->nb_args == 0)
		return (EXIT_FAILURE);
	while (cmd->args[i])
	{
		tmp = lst_env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->name, cmd->args[i], ft_strlen(tmp->name) + 1))
			{
				lst_del_env(lst_env, tmp->name);
				break ;
			}
			tmp = tmp->next;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
