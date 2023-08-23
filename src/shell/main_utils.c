/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksadykov <ksadykov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:31:34 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/23 21:15:49 by ksadykov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	failure(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

t_all	*build_all(t_env *lst_env)
{
	t_all		*all;
	t_border	*b;

	b = malloc(sizeof(t_border));
	if (!b)
		return (NULL);
	all = malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->ast = NULL;
	all->b = b;
	all->cmd = NULL;
	all->env = lst_env;
	all->count = 0;
	all->n_pipes = 0;
	all->prompt_good = ft_strdup("\001\033[35m\002(>.<) $ \001\033[0m\002");
	all->prompt_bad = ft_strdup("\001\033[31m\002(́o.o) $ \001\033[0m\002");
	return (all);
}

void	init_border(t_all **all, int *count)
{
	(*all)->count = *count;
	(*all)->b->end = *count - 1;
	*count = 0;
	(*all)->b->start = count;
}

void	get_status(t_all **all)
{
	int	i;
	int	status;

	i = 0;
	while (i < (*all)->count)
	{
		if ((*all)->cmd[i].pid > 0)
		{
			if (waitpid((*all)->cmd[i].pid, &status, 0) == -1)
				status = (*all)->cmd[i].status;
			if (WIFEXITED(status))
				*exit_status() = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*exit_status() = 128 + WTERMSIG(status);
			if ((*all)->cmd[i].redir_err != 0)
				*exit_status() = (*all)->cmd[i].redir_err;
		}
		// printf("MAIN : cmd : %s in : %d out : %d\n", (*all)->cmd[i].content, (*all)->cmd[i].input,(*all)->cmd[i].output);
		if ((*all)->cmd[i].input != STDIN_FILENO && (*all)->cmd[i].input > 0)
		{
			// printf("should be here\n");
			close((*all)->cmd[i].input);
		}
		if ((*all)->cmd[i].output != STDOUT_FILENO && (*all)->cmd[i].output > 0)
			close((*all)->cmd[i].output);
		i++;
	}
}

void	make_free(t_all **all, t_ast **root, t_cmd **cmds)
{
	if (*cmds)
	{
		free_cmds(*cmds, (*all)->count);
		(*all)->cmd = NULL;
		*cmds = NULL;
	}
	if (*root)
	{
		free_ast(*root);
		(*all)->ast = NULL;
		*root = NULL;
	}
}
