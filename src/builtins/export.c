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
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	exp_no_arg(int output_fd, t_env *lst_env)
{
	int		i;
	char	*res;
	char	**env;

	i = 0;
	env = env_to_tab(lst_env);
	while (env[i])
	{
		res = ft_strjoin("export ", env[i]);
		if (res)
		{
			ft_putendl_fd(res, output_fd);
			free(res);
		}
		i++;
	}
	free_tab(env);
	return (EXIT_FAILURE);
}

int	in_env(char *name, char *new_val, t_env *lst_env)
{
	t_env	*tmp;

	tmp = lst_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(new_val);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	exec_export(t_cmd *cmd, t_env **lst_env, int output_fd)
{
	char	**env;
	int		i;

	i = 1;
	if (!cmd)
		return (ft_putstr_fd("bash: export: problem\n", output_fd),
			EXIT_FAILURE);
	if (cmd->nb_args == 0)
		return (exp_no_arg(output_fd, *lst_env));
	while (cmd->args[i])
	{
		env = get_env(cmd->args[i]);
		if (check_env(env) == EXIT_FAILURE)
			return (env_error(env, output_fd));
		else if (in_env(env[0], env[1], *lst_env))
		{
			free_tab(env);
			i++;
			continue ;
		}
		add_back_env(lst_env, new_env(env[0], env[1]));
		free_tab(env);
		i++;
	}
	return (EXIT_SUCCESS);
}
