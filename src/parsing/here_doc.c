/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 18:01:46 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 11:51:48 by ejankovs         ###   ########.fr       */
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

void	ctrl_d(t_here **here, int *ctrl)
{
	if (!(*here)->line && *exit_here() != 130)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd((*here)->i, 2);
		ft_putstr_fd(" delimited by end-of-file\n", 2);
		*exit_here() = 131;
		close((*here)->fd);
		*ctrl = *exit_here();
		return ;
	}
	*ctrl = EXIT_SUCCESS;
}

int	analyze_exit_here(t_here **here)
{
	t_here	*h;

	h = *here;
	if (*exit_here() == 130)
	{
		if (h->line)
			free(h->line);
		return (close(h->fd), *exit_here());
	}
	if (*exit_here() == 131)
	{
		ft_putstr_fd("bash: warning: here-document at line ", 2);
		ft_putnbr_fd(h->i, 2);
		ft_putstr_fd(" delimited by end-of-file\n", 2);
		*exit_here() = 131;
		if (h->line)
			free(h->line);
		return (close(h->fd), *exit_here());
	}
	return (EXIT_SUCCESS);
}

int	read_stdin(int fd, t_token *t, t_env *env)
{
	t_here	*here;
	int		ctrl;

	ctrl = 0;
	here = NULL;
	if (init_here(&here, fd, &ctrl) != EXIT_SUCCESS)
		return (free_here(here), ctrl);
	while (here->line && ft_strncmp(here->line, t->content,
			ft_max(ft_strlen(here->line) - 1, ft_strlen(t->content))) != 0)
	{
		if (t->type == DREDIR2)
		{
			here->line = here_doc_expand(here->line, env);
			if (!here->line)
				return (free_here(here), close(fd), EXIT_FAILURE);
		}
		get_line(&here, &ctrl);
		if (ctrl != EXIT_SUCCESS)
			return (free_here(here), ctrl);
		here->i++;
	}
	ctrl = analyze_exit_here(&here);
	if (ctrl != EXIT_SUCCESS)
		return (free_here(here), ctrl);
	return (free_here(here), close(fd), EXIT_SUCCESS);
}

int	open_here_doc(int fd, t_token *t, t_all *all, t_token *tmp)
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
