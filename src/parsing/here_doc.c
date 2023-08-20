/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 05:22:24 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 05:30:166 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (i > 0 && line[i - 1] != '\n')
	{
		tmp = line;
		line = ft_strjoin(tmp, "\n");
		free(tmp);
	}
	return (line);
}

int	ctrl_d(char *line, int i, int fd)
{
	if (!line && *exit_here() != 130)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd(i, 2);
		ft_putstr_fd(" delimited by end-of-file\n", 2);
		*exit_here() = 131;
		return (close(fd), *exit_here());
	}
	return (EXIT_SUCCESS);
}

int	analyze_exit_here(char *line, int i, int fd)
{
	if (*exit_here() == 130)
	{
		if (line)
			free(line);
		return (close(fd), *exit_here());
	}
	if (*exit_here() == 131)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd(i, 2);
		ft_putstr_fd(" delimited by end-of-file\n", 2);
		*exit_here() = 131;
		if (line)
			free(line);
		return (close(fd), *exit_here());
	}
	return (EXIT_SUCCESS);
}

void	get_line(int fd, char *line, int i)
{
	int		ctrl;

	if (write(fd, line, ft_strlen(line)) < 0)
		failure("write failed");
	free(line);
	line = readline("> ");
}

int	ft_limit(char *line, char *limiter)
{
	if (ft_strncmp(line, limiter, ft_max(ft_strlen(line),
				ft_strlen(limiter))) != 0)
		return (0);
	return (1);
}

int	read_stdin(int fd, char *limiter, int type, t_env *env)
{
	char	*line;
	int		i;
	int		ctrl;

	i = 1;
	line = readline("> ");
	ctrl = ctrl_d(line, i, fd);
	if (ctrl != EXIT_SUCCESS)
		return (ctrl);
	while (line && !ft_limit(line, limiter))
	{
		if (type == DREDIR2)
		{
			line = here_doc_expand(line, env);
			if (!line)
				return (close(fd), EXIT_FAILURE);
		}
		get_line(fd, line, i);
		ctrl = ctrl_d(line, i, fd);
		if (ctrl != EXIT_SUCCESS)
			return (ctrl);
		i++;
	}
	ctrl = analyze_exit_here(line, i, fd);
	if (ctrl != EXIT_SUCCESS)
		return (ctrl);
	if (line)
		free(line);
	return (close(fd), EXIT_SUCCESS);
}

int	open_here_doc(int fd, t_token *t, t_all *all, t_token *tmp)
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
		here_child(fd, t, all, tmp);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*exit_status() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status() = WTERMSIG(status) + 128;
		if (*exit_status() == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
