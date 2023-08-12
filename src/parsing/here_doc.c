#include "minishell.h"

int	*heredoc(void)
{
	static int	exit_s = 0;

	return (&exit_s);
}

char	*here_doc_expand(char *line, t_env *env)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((size_t)i < ft_strlen(line) && line[i])
	{
		if (line[i] == '$')
		{
			line = expansion(line, &i, env, NULL);
			if (!line)
				return (NULL);
		}
		else
			i++;
	}
	if (line[i - 1] != '\n')
	{
		tmp = line;
		line = ft_strjoin(tmp, "\n");
		free(tmp);
	}
	return (line);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	read_stdin(int fd, char *limiter, int type, t_env *env)
{
	char	*line;

	*heredoc() = 1;
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
				ft_strlen(limiter))) != 0)
	{
		if (type == DREDIR2)
		{
			line = here_doc_expand(line, env);
			if (!line)
				return (EXIT_FAILURE);
		}
		if (write(fd, line, ft_strlen(line)) < 0)
			failure("write failed");
		free(line);
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	*heredoc() = 0;
	return (close(fd), EXIT_SUCCESS);
}

int	open_here_doc(int *pipe_fds, char *limiter, int type, t_env *env)
{
	if (pipe(pipe_fds) < 0)
		failure("pipe");
	return (read_stdin(pipe_fds[1], limiter, type, env));
}

void	here_doc_q(char *line, t_token **lst_tok)
{
	int		i;
	t_token	*last;

	i = 0;
	last = last_elem(*lst_tok);
	while (line[i] && !is_op(line + i) && !is_fb(line + i))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			last->type = DREDIR2_E;
			return ;
		}
		i++;
	}
}
