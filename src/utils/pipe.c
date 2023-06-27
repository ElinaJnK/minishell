#include "../../minishell.h"

void	close_pipes(int *pipe_fds, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes * 2)
	{
		close(pipe_fds[i]);
		i++;
	}
}

void	generate_pipes(int *pipe_fds, int num_pipes)
{
	int	i;

	i = 0;
	pipe_fds = (int *) malloc(sizeof(int) * num_pipes * 2);
	if (!pipe_fds)
		failure("Error in pipe generation");
	while (i < num_pipes)
	{
		if (pipe(pipe_fds + (i * 2)) < 0)
			failure("Error in pipe generation");
		i++;
	}
}