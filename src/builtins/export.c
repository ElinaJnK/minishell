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
	if (!env || !env[0] || !env[1])
		return (EXIT_FAILURE);
	if (env[2] || check_name(env[0]) == EXIT_FAILURE)
	//	|| check_name(env[1]) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	put_string(char **str, int output_fd)
{
	int		i;
	char	*res;

	i = 0;
	while (str[i])
	{
		res = ft_strjoin("export ", str[i]);
		if (res)
		{
			ft_putendl_fd(res, output_fd);
			free(res);
		}
		i++;
	}
}

int		exec_export(t_cmd *cmd, t_env **lst_env, int output_fd)
{
	char	**env;
	char	**env_error;
	int		i;

	env = NULL;
	i = 1;
	if (!cmd || cmd->nb_args == 0)
	{
		env_error = env_to_tab(*lst_env);
		return (put_string(env_error, output_fd), free_tab(env_error), EXIT_FAILURE);
	}
	while (cmd->args[i])
	{
		env = get_env(cmd->args[i]);
		if (check_env(env) == EXIT_FAILURE)
		{
			free_tab(env);
			return (ft_putstr_fd("bash: export: not a valid identifier\n", output_fd), EXIT_FAILURE);
		}
		add_back_env(lst_env, new_env(env[0], env[1]));
		free_tab(env);
		i++;
	}
	return (EXIT_SUCCESS);
}
