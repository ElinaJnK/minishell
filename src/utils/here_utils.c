#include "minishell.h"

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
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

void	here_child(int fd, t_token *t, t_all *all, t_token *tmp)
{
	int	ex;

	sig_child();
	signal_here_c();
	ex = read_stdin(fd, t->content, t->type, all->env);
	free_lst_tok(&tmp);
	free_all(all);
	exit(ex);
}
