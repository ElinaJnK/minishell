#include "minishell.h"

static void handle_here(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n\nSIGINT\n\n");
		*exit_status() = 130;
		exit(*exit_status());
	}
	else if (sig == SIGQUIT)
	{
		printf("\n\nQUIT\n\n");
		ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
		*exit_status() = 131;
		signal(SIGQUIT, SIG_IGN);
	}
}

void	signal_here(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
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
			line = expansion_here(line, &i, env, NULL);
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

	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
				ft_strlen(limiter))) != 0)
	{
		signal_here();
		if (*exit_status() == 130 || *exit_status() == 131)
			{printf("\n\nHERE\n\n"); break ;}
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
	return (close(fd), EXIT_SUCCESS);
}

int	open_here_doc(int *pipe_fds, char *limiter, int type, t_env *env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	status = 0;
	if (pid == -1)
	{
		perror("fork");
        exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		sig_child();
		read_stdin(pipe_fds[1], limiter, type, env);
		exit(*exit_status());
	}
	else
	{
		waitpid(pid, &status, 0);
		*exit_status() = status;
	}
	return (EXIT_SUCCESS);
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
