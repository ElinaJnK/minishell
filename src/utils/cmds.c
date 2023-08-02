#include "minishell.h"

void	free_cmds(t_cmd *cmds, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (cmds)
	{
		while (i < count)
		{
			j = 0;
			if (cmds[i].args)
			{
				while (cmds[i].args[j] && j <= cmds[i].nb_args)
				{
					free(cmds[i].args[j]);
					j++;
				}
				free(cmds[i].args);
			}
			if (cmds[i].content)
				free(cmds[i].content);
			i++;
		}
		free(cmds);
	}
}

int	lst_size_tok(t_token *lst)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = lst;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int	init_op(t_cmd *cmd, t_token *t)
{
	if (t)
	{
		cmd->content = ft_strdup(t->content);
		if (!cmd->content)
			return (EXIT_FAILURE);
		cmd->type = t->type;
	}
	else
	{
		cmd->content = NULL;
		cmd->type = -1;
	}
	cmd->output = STDOUT_FILENO;
	cmd->input = STDIN_FILENO;
	cmd->args = NULL;
	cmd->nb_args = 0;
	return (EXIT_SUCCESS);
}
