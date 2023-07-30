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
	//printf("\n\nELEM[0] = %s\n\n\n", elem[0]);
	elem[1] = ft_strdup(data + i + 1);
	if (!elem[1])
	{
		free(elem[0]);
		free(elem);
		failure_env("malloc of elem[1] is NULL", elem);
		return (NULL);
	}
	return (elem);
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
		// int	j;

		// j = 0;
		// while (elem && elem[j])
		// 	free(elem[j++]);
		if (elem)
			free(elem);
		i++;
	}
	return (lst_env);
}

/*-------------------------------------TESTS-----------------------*/


// int main(int ac, char **av, char **env)
// {
// 	// print the env
// 	t_env *lst_env;

// 	(void)ac;
// 	(void)av;
// 	lst_env = spy_env(env);
// 	print_list_env(lst_env);
// 	free_lst_env(&lst_env);
// 	return (0);
// }