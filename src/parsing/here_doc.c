#include "minishell.h"

int	*exit_here(void)
{
	static int	here = 0;

	return (&here);
}

static void handle_here_c(int sig)
{
	(void)sig;
	//printf("\n\nSIGINT\n\n");
	close(0);
	*exit_here() = 130;

	// if (*pid_child() != -1)
	// 	kill(*pid_child(), SIGINT);
}

static void handle_here_d(int sig)
{
	(void)sig;
	printf("\n\nQUIT\n\n");
	close(0);
	//ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
	*exit_here() = 131;
	//signal(SIGQUIT, SIG_IGN);
}

void	signal_here_c(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here_c;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
}

void	signal_here_d(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here_d;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
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

	signal_here_c();
	signal_here_d();
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	if (!line && *exit_here() == 131)  // EOF detected
	{
		ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
		return (*exit_here()); 
	}
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
		if (*exit_here() == 131)  // EOF detected
		{
			ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
			return (*exit_here()); 
		}
	}
	if (*exit_here() == 130)
		return (*exit_here());
	if (*exit_here() == 131)  // EOF detected
	{
		ft_putstr_fd("bash: warning: here-document delimited by end-of-file\n", 2);
		return (*exit_here()); 
	}
	if (line)
		free(line);
	return (close(fd), EXIT_SUCCESS);
}

// static void	handle_sigint_child(int sig)
// {
// 	(void)sig;
// 	*exit_status() = 130;
// 	exit(*exit_status());
// }

int	open_here_doc(int *pipe_fds, char *limiter, int type, t_env *env)
{
	pid_t	pid;
	int		status;
	int		ex;

	pid = fork();
	status = 0;
	ex = 0;
	if (pid == -1)
	{
		perror("fork");
        exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		sig_child();
		//signal(SIGINT, handle_sigint_child);
		ex = read_stdin(pipe_fds[1], limiter, type, env);
		exit(ex);
	}
	else
	{
		//printf("\n\nLOLO\n\n");
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status() = WTERMSIG(status) + 128;
		if (*exit_status() == EXIT_FAILURE)
			return (EXIT_FAILURE);
		//printf("\n\nLOLO2\n\n");
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
