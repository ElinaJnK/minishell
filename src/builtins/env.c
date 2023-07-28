#include "minishell.h"

void	free_tab(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

int	exec_env(t_cmd *cmd, int fd_out, t_env *lst_env)
{
	int		i;
	char	**res;

	i = 0;
	if (!cmd || !lst_env)
		return (failure_exec("No environment"), EXIT_FAILURE);
	res = env_to_tab(lst_env);
	if (!res)
		return (EXIT_FAILURE);
	while (res[i])
	{
		ft_putendl_fd(res[i], fd_out);
		i++;
	}
	free_tab(res);
	return (EXIT_SUCCESS);
}