#include "minishell.h"

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
	el->name = ft_strdup(name);
	el->value = ft_strdup(value);
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

void	del_elem(t_env **lst_env, t_env *temp, t_env *prev)
{
	if (prev)
		prev->next = temp->next;
	else
		*lst_env = temp->next;
	if (temp->name)
		free(temp->name);
	if (temp->value)
		free(temp->value);
	free(temp);
	if (prev)
		temp = prev->next;
	else
		temp = *lst_env;
}

void	lst_del_env(t_env **lst_env, char *name)
{
	t_env	*temp;
	t_env	*prev;

	temp = *lst_env;
	prev = NULL;
	if (!*lst_env || !name)
		return ;
	while (temp)
	{
		if (ft_strncmp(temp->name, name, ft_strlen(name) + 1) == 0)
		{
			del_elem(lst_env, temp, prev);
			break ;
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}	
