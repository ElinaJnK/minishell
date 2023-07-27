#include "minishell.h"

void	free_cmds(t_cmd *cmds, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (cmds)
	{
		while (i < count)
		{
			j = 0;
			if (cmds[i].args)
			{
				while (cmds[i].args[j] && j < cmds[i].nb_args)
				{
					free(cmds[i].args[j]);
					j++;
				}
				free(cmds[i].args);
			}
			if (cmds[i].content)
				free(cmds[i++].content);
		}
		free(cmds);
	}
}
