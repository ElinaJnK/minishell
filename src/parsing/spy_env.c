#include "minishell.h"

void	failure_env(const char *message, char **elem)
{
	int	i;

	i = 0;
	while (elem && elem[i])
		free(elem[i]);
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
	elem = malloc(sizeof(char *) * 3);
	if (!elem || !data)
	{
		failure_env("malloc of elem, data is NULL", NULL);
	}
	elem[2] = NULL;
	while (data[i] && data[i] != '=')
		i++;
	elem[0] = malloc(sizeof(char) * (i + 1));
	if (!elem[0])
	{
		failure_env("malloc of elem[0] is NULL", elem);
	}
	ft_strlcpy(elem[0], data, i + 1);
	elem[1] = ft_strdup(data + i + 1);
	return (elem);
}

t_env	*spy_env(char **env)
{
	t_env	*lst_env;
	char	**elem;
	int		i;
	int		j;

	i = 0;
	j = 0;
	lst_env = NULL;
	if (!env)
		return (NULL);
	while (env && env[i])
	{
		elem = get_env(env[i]);
		add_back_env(&lst_env, new_env(ft_strdup(elem[0]), ft_strdup(elem[1])));
		j = 0;
		while (j < 3)
			free(elem[j++]);
		free(elem);
		i++;
	}
	return (lst_env);
}

/*-------------------------------------TESTS-----------------------*/
// void print_list_env(t_env *lst_env)
// {
// 	t_env *temp;

// 	temp = lst_env;
// 	while (temp)
// 	{
// 		printf("%s  =   %s", temp->name, temp->value);
// 		// printf("[%s]->", temp->content);
// 		temp = temp->next;
// 		printf("\n");
// 	}
// }

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