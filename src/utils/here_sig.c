/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_sig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejankovs <ejankovs@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 20:04:11 by ksadykov          #+#    #+#             */
/*   Updated: 2023/08/20 20:04:11 by ejankovs         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*exit_here(void)
{
	static int	here = 0;

	return (&here);
}

void	handle_here_c(int sig)
{
	(void)sig;
	close(0);
	*exit_here() = 130;
}

void	signal_here_c(void)
{
	struct sigaction	act;

	act.sa_handler = handle_here_c;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
}

void	get_line(t_here **h, int *ctrl)
{
	t_here	*here;

	here = *h;
	if (write(here->fd, here->line, ft_strlen(here->line)) < 0)
		failure("write failed");
	free(here->line);
	here->line = readline("> ");
	ctrl_d(&here, ctrl);
}
