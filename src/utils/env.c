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
