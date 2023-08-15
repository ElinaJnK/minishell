#include "minishell.h"

void	failure_env(const char *message, char **elem)
{
	int	i;

	i = 0;
	while (elem && elem[i])
		free(elem[i++]);
	if (elem)
		free(elem);
	perror(message);
	exit(EXIT_FAILURE);
}

char	**env_to_tab(t_env *lst_env)
{
	char	**env;
	t_env	*tmp;
	int		i;
	char	*str;

	env = (char **)malloc(sizeof(char *) * (lst_size_env(lst_env) + 1));
	if (!env)
		return (failure("lst_env: env not properly malloc'd"), NULL);
	tmp = lst_env;
	i = 0;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, "", 1) == 0)
			env[i] = ft_strdup(tmp->name);
		else
		{
			str = ft_strjoin(tmp->name, "=");
			env[i] = ft_strjoin(str, tmp->value);
			free(str);
		}
		tmp = tmp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

char	**get_value(char **elem, char *data, int i)
{
	if (data[i] == '=' && data[i + 1])
	{
		elem[1] = ft_strdup(data + i + 1);
		if (!elem[1])
		{
			free(elem[0]);
			free(elem);
			failure_env("malloc of elem[1] is NULL", elem);
			return (NULL);
		}
	}
	else
		elem[1] = ft_strdup("");
	return (elem);
}

char	**get_env(char *data)
{
	char	**elem;
	int		i;

	i = 0;
	if (!data)
		return (NULL);
	elem = malloc(sizeof(char *) * 3);
	if (!elem)
		return (failure_env("malloc of elem, data is NULL", NULL), NULL);
	elem[2] = NULL;
	while (data[i] && data[i] != '=')
		i++;
	elem[0] = malloc(sizeof(char) * (i + 1));
	if (!elem[0])
	{
		free(elem);
		failure_env("malloc of elem[0] is NULL", elem);
		return (NULL);
	}
	ft_strlcpy(elem[0], data, i + 1);
	return (get_value(elem, data, i));
}

int	in_env_bis(char *name, t_env *lst_env)
{
	t_env	*tmp;
	int		num;

	tmp = lst_env;
	num = 0;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name) + 1) == 0)
		{
			if (is_num(tmp->value) == EXIT_SUCCESS)
			{
				num = ft_atoi(tmp->value);
				free(tmp->value);
				++num;
				tmp->value = ft_itoa(num);
			}
			else
			{
				free(tmp->value);
				tmp->value = ft_strdup("1");
			}
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

t_env	*spy_env(char **env)
{
	t_env	*lst_env;
	char	**elem;
	int		i;

	i = 0;
	lst_env = NULL;
	if (!env)
		return (NULL);
	while (env && env[i])
	{
		elem = get_env(env[i]);
		if (!elem)
			return (NULL);
		add_back_env(&lst_env, new_env(elem[0], elem[1]));
		free_tab(elem);
		i++;
	}
	if (!in_env_bis("SHLVL", lst_env))
		add_back_env(&lst_env, new_env("SHLVL", "1"));
	return (lst_env);
}
