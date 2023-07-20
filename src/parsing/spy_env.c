#include "minishell.h"

void	failure_env(const char *message, char **elem)
{
	int	i;

	i = 0;
	while (elem && elem[i])
		free(elem[i]);
	free(elem);
	perror(message);
	exit(EXIT_FAILURE);
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
		elem = ft_split(env[i], '=');
		if (elem[2])
			return (failure_env("env variable has > 1 \' = \' character ", elem), NULL);
		add_back_env(&lst_env, new_env(elem[0], elem[1]));
		i++;
	}
	return (lst_env);
}

/*-------------------------------------TESTS-----------------------*/
void print_list_env(t_env *lst_env)
{
	t_env *temp;

	temp = lst_env;
	while (temp)
	{
		printf("%s=%s", temp->name, temp->value);
		// printf("[%s]->", temp->content);
		temp = temp->next;
		printf("\n");
	}
}

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