#include "minishell.h"

void	free_cmds(t_cmd *cmds, int count)
{
	int	i;
	int	j;

	i = 0;
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
		if (cmds->lst_err)
			free_lst_tok(&(cmds->lst_err));
		free(cmds);
		cmds = NULL;
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
	cmd->lst_err = NULL;
	cmd->args = NULL;
	cmd->nb_args = 0;
	cmd->n_pipes = 0;
	cmd->pid = -42;
	return (EXIT_SUCCESS);
}

int	is_builtin(t_cmd *cmd)
{
	if (!ft_strncmp(cmd->content, "cd", ft_strlen("cd") + 1)
		|| !ft_strncmp(cmd->content, "echo", ft_strlen("echo") + 1)
		|| !ft_strncmp(cmd->content, "env", ft_strlen("env") + 1)
		|| !ft_strncmp(cmd->content, "exit", ft_strlen("exit") + 1)
		|| !ft_strncmp(cmd->content, "export", ft_strlen("export") + 1)
		|| !ft_strncmp(cmd->content, "pwd", ft_strlen("pwd") + 1)
		|| !ft_strncmp(cmd->content, "unset", ft_strlen("unset") + 1))
		return (1);
	return (0);
}

void	add_error(char *filename, int fd_out, t_token **lst_err)
{
	char	*error;

	if (access(filename, R_OK) != 0)
	{
		if (errno == ENOENT)
		{
			error = ft_strjoin(filename, ": No such file or directory\n");
			*exit_status() = 127;
		}
		else
		{
			error = ft_strjoin(filename, ": Permission denied\n");
			*exit_status() = 126;
		}
		add_back_tok(lst_err, new_token(error, fd_out));
	}
}
