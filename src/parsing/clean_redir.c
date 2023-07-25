#include "minishell.h"

char	*get_content(int fd)
{
	char	*cur;
	char	*inter;
	char	*res;

	res = NULL;
	cur = get_next_line(fd);
	while (line && ft_strncmp(line, limiter, ft_max(ft_strlen(line) - 1,
				ft_strlen(limiter))) != 0)
	{
		res = ft_strjoin(inter, cur);
		free(inter);
		free(cur);
		inter = res;
		cur = get_next_line(fd);
	}
	if (!res)
		return (cur);
	return (res);
}

t_token	*heredoc(t_token *head)
{
	int		pipe_fds[2];
	t_token	*tmp;
	t_token	*new;

	tmp = head;
	new = NULL;
	while (tmp && tmp->next && (tmp->type == DREDIR2 || tmp->type == DREDIR2_E || tmp->type == CMD))
	{
		if (tmp->next->type == CMD)
		{
			open_here_doc(pipe_fds, tmp->next->content);
			tmp
		}
		else
			return (failure_parse("on ecrira apres", head), NULL);
		tmp = tmp->next->next;
	}
	new = new_token(get_content(pipe_fds[1]), DREDIR2);
	new->next = tmp;
	return (new);
}

t_token *redir(t_token *t)
{
	t_token *tmp;

	tmp = t;
	while (tmp && (tmp->type == REDIR || tmp->type == CMD))
}

t_token	*clean_up(t_token *t)
{
	t_token	*tmp;

	tmp = t;
	if (!t)
		return (NULL);
	while (tmp)
	{
		if (tmp->type == DREDIR2 || tmp->type == DREDIR2_E)
			tmp->next = heredoc(tmp);
		if (tmp->type == REDIR)
			tmp->next = redir(tmp);
		tmp = tmp->next;
	}
}