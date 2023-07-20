#include "minishell.h"

void	expand(t_cmd *cmds, int size)
{
	int	i;

	i = 0;
	while (cmds && cmds[i] && i < size)
	{
		if (cmds[i].type == CMD)
		{
			
		}
		i++;
	}
}
