#include "minishell.h"

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	read_stdin(int fd, char *limiter)
{
	char	*line;

	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
				ft_strlen(limiter))) != 0)
	{
		if (write(fd, line, ft_strlen(line)) < 0)
			failure("write failed");
		free(line);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	close(fd);
}

int	open_here_doc(int *pipe_fds, char *limiter)
{
	if (pipe(pipe_fds) < 0)
		failure("pipe");
	read_stdin(pipe_fds[1], limiter);
	return (EXIT_SUCCESS);
}
