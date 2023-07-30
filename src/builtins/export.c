#include "minishell.h"

int	check_name(char *name)
{
	int	i;

	i = 0;
	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (EXIT_FAILURE);
	while (name[i])
	{
		if (!ft_isalpha(name[i]) && name[i] != '_'
			&& !ft_isdigit(name[i]))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	check_env(char **env)
{
	if (!env)
		return (EXIT_FAILURE);
	if (env[2] || check_name(env[0]) == EXIT_FAILURE
		|| check_name(env[1]) == EXIT_FAILURE)
		return (free(env), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		exec_export(t_cmd *cmd, t_env **lst_env)
{
	char	**env;
	int		i;

	env = NULL;
	i = 1;
	if (!cmd || cmd->nb_args == 0)
		return (failure_exec("bash"), EXIT_FAILURE);
	while (cmd->args[i])
	{
		env = get_env(cmd->args[i]);
		if (check_env(env) == EXIT_FAILURE)
			return (failure_exec("bash"), EXIT_FAILURE);
		add_back_env(lst_env, new_env(env[0], env[1]));
		free(env);
		i++;
	}
	return (EXIT_SUCCESS);
}
