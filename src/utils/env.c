#include "minishell.h"

int	lst_size_env(t_env *lst)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
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
		str = ft_strjoin(tmp->name, "=");
		env[i] = ft_strjoin(str, tmp->value);
		if (str)
			free(str);
		tmp = tmp->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

void	add_back_env(t_env **lst_env, t_env *new)
{
	t_env	*tmp;

	if (!lst_env)
		failure("lst_env is NULL");
	if (*lst_env == NULL)
	{
		*lst_env = new;
		return ;
	}
	tmp = *lst_env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*new_env(char *name, char *value)
{
	t_env	*el;

	el = malloc(sizeof(t_env));
	if (!el)
		failure("malloc error");
	el->name = name;
	el->value = value;
	el->next = NULL;
	return (el);
}

void	free_lst_env(t_env **lst_env)
{
	t_env	*tmp;

	if (!lst_env)
		return ;
	while (*lst_env)
	{
		tmp = (*lst_env)->next;
		free((*lst_env)->name);
		free((*lst_env)->value);
		free(*lst_env);
		*lst_env = tmp;
	}
}
