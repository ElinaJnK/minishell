/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:18:52 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/22 19:53:10 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	just_do_it(t_all **all, char *line, int count)
{
	t_cmd		*cmds;
	t_ast		*root;
	t_token		*t;

	cmds = NULL;
	root = NULL;
	t = tokenize(line, (*all)->env);
	t = tokenize_bise(t);
	t = tokenize_crise(t, (*all)->env);
	if (t && (*all)->env)
		cmds = transform_into_tab(t, &count, all);
	if (cmds)
	{
		init_border(all, &count);
		root = build_ast(cmds, (*all)->b);
		//printAST(root);
		if (root && (*all)->env)
		{
			(*all)->cmd = cmds;
			(*all)->ast = root;
			exec_ast(&((*all)->ast), STDIN_FILENO, STDOUT_FILENO, *all);
			get_status(all);
		}
		make_free(all, &root, &cmds);
	}
}

void	tchitat_stdin(t_all **all)
{
	char		*line;
	int			count;

	while (1)
	{
		signal_prompt();
		if (*exit_status() == 0)
			line = readline((*all)->prompt_good);
		else
			line = readline((*all)->prompt_bad);
		if (!line)
			return ;
		add_history(line);
		count = 0;
		if (*line == '\n' || *line == '\0')
		{
			free(line);
			line = NULL;
		}
		signal_exec();
		if (line)
		{
			(*all)->n_pipes = 0;
			just_do_it(all, line, count);
		}
	}
}

void	finish_main(t_all **a)
{
	t_all	*all;

	all = *a;
	if (all->env)
	{
		free_lst_env(&all->env);
		all->env = NULL;
	}
	free(all->b);
	free(all->prompt_good);
	free(all->prompt_bad);
	free(all);
}

int	main(int ac, char **av, char **env)
{
	t_env		*lst_env;
	t_all		*all;

	(void)ac;
	(void)av;
	rl_outstream = stderr;
	//*exit_status();
	if (ac != 1 || av[1])
	{
		ft_putstr_fd("This program does not accept arguments\n", 2);
		exit(0);
	}
	init_env(env, &lst_env, &all);
	if (!lst_env)
		return (failure("Error in malloc allocation"), 1);
	if (!all)
		return (failure("Error in malloc allocation"), free_lst_env(&lst_env),
			*exit_status());
	tchitat_stdin(&all);
	finish_main(&all);
	return (rl_clear_history(), *exit_status());
}
