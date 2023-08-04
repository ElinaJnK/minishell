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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}

int	env_error(char **env, int output_fd)
{
	ft_putstr_fd("bash: export: ", output_fd);
	if (env[0])
		ft_putstr_fd(env[0], output_fd);
	ft_putstr_fd(": not a valid identifier\n", output_fd);
	free_tab(env);
	return (EXIT_FAILURE);
}

int	exec_env(t_cmd *cmd, int fd_out, t_env *lst_env)
{
	t_env	*tmp;

	if (!cmd || !lst_env)
		return (ft_putstr_fd("bash: env: env not defined\n", fd_out),
			EXIT_FAILURE);
	tmp = lst_env;
	while (tmp)
	{
		if (ft_strncmp(tmp->value, "", 1) != 0)
		{
			ft_putstr_fd(tmp->name, fd_out);
			ft_putstr_fd("=", fd_out);
			ft_putendl_fd(tmp->value, fd_out);
		}
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}
